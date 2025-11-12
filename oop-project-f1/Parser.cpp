//
// Created by luka on 12/26/24.
//

#include "Exceptions.h"
#include "Parser.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

using namespace std;

Parser::Parser(Reader *reader) : mReader(reader) {}

Parser::~Parser() {
    delete mReader;
}

void Parser::setInsidePipeline(const bool status) {
    insidePipeline = status;
}

string Parser::getLine() const {
    string line = mReader->getNextLine();
    line = cleanStr(line); // should never return and empty string
    if (mReader->endOfRead()) return "";
    return line;
}

string Parser::cleanStr(const string &str) {
    string final;
    bool isMidSpace = false;

    for (const char c : str) {
        if (isspace(c)) {
            if (!isMidSpace) {
                isMidSpace = true;
                final += ' ';
            }
        }
        else {
            final += c;
            isMidSpace = false;
        }
    }

    if (!final.empty() && isspace(final[0])) {
        final = final.substr(1);
    }

    return final;
}

bool Parser::validCommand(const string& line) {
    if (!invalidPositions.empty()) {
        parserErr("unexpected character(s):" , line);
        return false;
    }
    return true;
}

string Parser::parseCommand(const string &line, unsigned *index) {
    string command, parseLine = line;
    if (*index < line.size() && isspace(parseLine[*index])) parseLine = line.substr(1);
    while (*index < parseLine.size()) {
        const char c = parseLine[*index];
        if (isspace(c)) break;
        if (!islower(c) && !isupper(c)) {
            invalidPositions.push_back(*index);
        }
        command += c;
        (*index)++;
    }
    (*index)++;
    return command;
}

string Parser::parseOpt(const string &line, unsigned *index) {
    string options;
    if (*index < line.size() && isspace(line[*index])) (*index)++;
    if (*index < line.size() && line[*index] != '-') return "";
    (*index)++;
    while (*index < line.size()) {
        const char c = line[*index];
        if (isspace(c)) break;
        if (!islower(c) && !isupper(c) && !isdigit(c)) {
            invalidPositions.push_back(*index);
        }
        options += c;
        (*index)++;
    }
    (*index)++;
    return options;
}

CommandArgs *Parser::parseArgs(const string &line, unsigned *index, const string& cmdName) {
    auto *args = new CommandArgs();
    if (*index > line.size() && insidePipeline) return args;

    if (line[*index] == '"') {  // console argument
        args->setContext(parseArgsConsole(line, index));
        parseRedirect(line, index, *args); // >(>)filepath
        return args;
    }

    if (islower(line[*index]) || isupper(line[*index]) || line[*index] == '<' /**index < line.size()*/) { // file arguments
        string filepath = parseFilepath(line, index);

        if (cmdName == "touch" || cmdName == "truncate" || cmdName == "rm") { // cmds which use just the file path
            args->setFilepath(filepath);
            return args;
        }

        if (filepath[0] == '<') filepath = filepath.substr(1);
        if (isspace(filepath[0])) filepath = filepath.substr(1);

        if (cmdName == "echo" || cmdName == "wc" || cmdName == "tr" || cmdName == "head" || cmdName == "batch") { // return file content in context
            args->setContext(parseArgsFile(filepath));
            parseRedirect(line, index, *args); // >(>)filepath
            return args;
        }

        parseRedirect(line, index, *args);
        return args;
    }

    //redirect
    parseRedirect(line, index, *args); // >(>)filepath

    if (insidePipeline) return args;

    if (*index >= line.size() && (cmdName == "echo" || cmdName == "wc" || cmdName == "tr" || cmdName == "head" || cmdName == "batch")) { // cmds which take multiline arguments
        args->setContext(parseConsoleTillEOF());
        return args;
    }

    return args; // return empty args
}

CommandArgs *Parser::parseTrArgs(const string &line, unsigned *index) {
    auto *args = new CommandArgs();
    vector<string> argsArr;

    if (isspace(line[*index])) (*index)++;
    if (islower(line[*index]) || isupper(line[*index]) || line[*index] == '<') { // handle file
        const string filepath = parseFilepath(line, index);
        const string context = parseArgsFile(filepath);
        argsArr.push_back(context);
    }

    stringstream ss(line.substr(*index));

    string arg;
    getline(ss, arg, '\"');

    while (getline(ss, arg, '\"')) {
        ss.ignore(511, '\"');
        argsArr.push_back(arg);
    }

    // handle redirect
    while (*index < line.size() && line[*index] != '>') (*index)++;
    string last = line.substr(*index);
    if (!last.empty() && last[0] == '>') {
        bool append = false;
        last = last.substr(1);

        if (!last.empty() && last[0] == '>') {
            append = true;
            last = last.substr(1);
        }

        if (isspace(last[0])) last = last.substr(1);
        args->redirectOutput(last, append);
    }

    if (insidePipeline) {
        if (argsArr.size() == 1) {
            args->setWhat(argsArr.at(0));
        }
        else if (argsArr.size() == 2) {
            args->setWhat(argsArr.at(0));
            args->setWith(argsArr.at(1));
        }
        else throw UnexpectedArgumentContext();
    }
    else {
        if (argsArr.size() == 1) {
            args->setWhat(argsArr.at(0));
            args->setContext(parseConsoleTillEOF());
        }
        else if (argsArr.size() == 2) {
            args->setContext(argsArr.at(0));
            args->setWhat(argsArr.at(1));
        }
        else if (argsArr.size() == 3) {
            args->setContext(argsArr.at(0));
            args->setWhat(argsArr.at(1));
            args->setWith(argsArr.at(2));
        }
        else throw UnexpectedArgumentContext();
    }

    return args;
}

void Parser::parserErr(const string &message, const string &line) {
    cout << "Error - " << message << endl;
    if (line.empty() && invalidPositions.empty()) return;
    cout << line << endl;
    unsigned j = 0;
    for (unsigned i = 0; !invalidPositions.empty() && i < line.length(); i++) {
        if (i == invalidPositions.at(j)) {
            cout << '^';
            j = j < invalidPositions.size() - 1 ? j + 1 : invalidPositions.size() - 1;
        }
        else cout << ' ';
    }
    cout << endl;

    invalidPositions.clear();
}

string Parser::parseArgsConsole(const string &line, unsigned *index) {
    string argument; unsigned quoteSeen = 0;
    quoteSeen++;
    (*index)++;

    while (*index < line.length()) {
        const char c = line[*index];
        if (c == '"') {
            quoteSeen++;
            break;
        }
        argument += c;
        (*index)++;
    }
    (*index)++;

    if (quoteSeen != 2 && !argument.empty()) {
        parserErr("malformed argument, try executing...", "");
    }

    return argument;
}

string Parser::parseArgsFile(const string& filepath) {
    Reader *oldConsoleReader = mReader;
    mReader = new FileReader(filepath);
    string context;

    if (!mReader->checkFile()) {
        delete mReader;
        mReader = oldConsoleReader;
        throw CouldNotOpenFile(filepath);
    }

    if (!mReader->endOfRead()) context = mReader->getNextLine();

    while (!mReader->endOfRead()) {
        context += '\n';
        context += mReader->getNextLine();
    }

    delete mReader;
    mReader = oldConsoleReader;
    return context;
}

string Parser::parseConsoleTillEOF() const {
    string line;
    while (!mReader->endOfRead()) {
        string input = mReader->getNextLine();
        if (input == "!EOF") break; // workaround for clion and EOF
        line += (input + '\n');
    }
    cin.clear();
    return line;
}

void Parser::parseRedirect(const string &line, unsigned *index, CommandArgs &cmdArgs) {
    if (isspace(line[*index])) (*index)++; // remove the first space char
    if (line[*index] && line[*index] == '>') {
        (*index)++; // remove first > char form filepath
        if (isspace(line[*index])) (*index)++; // remove space if exists between char > and filepath
        string output;
        bool append = false;

        while (*index < line.size()) {
            const char c = line[*index];
            if (c == '>') {
                append = true;
                (*index)++;
                if (isspace(line[*index])) (*index)++;
                continue;
            }
            output += c;
            (*index)++;
        }

        if (output.empty()) return;

        cmdArgs.redirectOutput(output, append);
    }

}

string Parser::parseFilepath(const string &line, unsigned *index) {
    string output;
    const unsigned save = *index;
    while (*index < line.size()) {
        const char c = line[*index];
        if (isspace(c) && save + 1 != *index) break;
        output += c;
        (*index)++;
    }

    return output;
}
