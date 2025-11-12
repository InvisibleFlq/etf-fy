//
// Created by luka on 12/26/24.
//

#ifndef PARSER_H
#define PARSER_H

#include "Reader.h"
#include "Command.h"
#include <vector>

class Parser {
public:
    explicit Parser(Reader *reader = new ConsoleReader()); // by default read from console (stdin)
    ~Parser();

    [[nodiscard]] string getLine() const;
    bool validCommand(const string &line);
    void setInsidePipeline(bool status);

    // can't be made static because of invalidPositions vector they modify
    string parseCommand(const string &line, unsigned *index);
    string parseOpt(const string &line, unsigned *index);
    CommandArgs *parseArgs(const string &line, unsigned *index, const string &cmdName);
    CommandArgs *parseTrArgs(const string &line, unsigned *index);

protected:
    static string cleanStr(const string &str);

    string parseArgsConsole(const string &line, unsigned *index);
    string parseArgsFile(const string &filepath);

    static void parseRedirect(const string &line, unsigned *index, CommandArgs &cmdArgs);
    static string parseFilepath(const string &line, unsigned *index);
    [[nodiscard]] string parseConsoleTillEOF() const;

    void parserErr(const string &message, const string &line);

private:
    Reader *mReader;
    bool insidePipeline = false;
    vector<unsigned> invalidPositions;
};

#endif //PARSER_H
