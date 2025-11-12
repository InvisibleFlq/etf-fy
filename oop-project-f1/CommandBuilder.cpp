//
// Created by lukav on 20.10.2025.
//

#include "./commands/ImportCommands.h"
#include "CommandBuilder.h"
#include "Exceptions.h"

#include <sstream>
#include <string>
#include <memory>

CommandBuilder::CommandBuilder(Parser *parser) : mParser(parser) {}

CommandBuilder::~CommandBuilder() {
    delete mParser;
}

Command *CommandBuilder::getCommand(const string &line) const {
    return assembleCommand(line);
}

Command *CommandBuilder::assembleCommand(const string &line) const {
    if (line.empty()) return nullptr;
    unsigned index = 0;

    const string commandName = mParser->parseCommand(line, &index);
    const string commandOpt = mParser->parseOpt(line, &index);

    CommandArgs *commandArgs = nullptr;
    if (commandName == "tr") commandArgs = mParser->parseTrArgs(line, &index);
    else commandArgs = mParser->parseArgs(line, &index, commandName);

    if (!mParser->validCommand(line)) {
        delete commandArgs;
        return nullptr;
    }

    Command *command = nullptr;

    try {
        command = buildCommand(commandName, commandOpt);

        command->setCmd(commandName);

        if (!commandOpt.empty()) {
            command->setOpt(commandOpt);
        }

        command->setArgs(commandArgs);
    }
    catch (const string &e) {
        delete command;
        delete commandArgs;
        throw;
    }

    return command;
}

Command *CommandBuilder::buildCommand(const string &commandName, const string &commandOptions){
    if (commandName.empty()) throw UnknownCommand(commandName);

    if (commandName == "exit") {
        if (commandOptions.empty()) return new ExitCommand();
        throw UnexpectedOptions();
    }

    if (commandName == "batch") {
        if (commandOptions.empty()) return new BatchCommand();
        throw UnexpectedOptions();
    }

    if (commandName == "date") {
        if (commandOptions.empty()) return new DateCommand();
        throw UnexpectedOptions();
    }

    if (commandName == "echo") {
        if (commandOptions.empty()) return new EchoCommand();
        throw UnexpectedOptions();
    }

    if (commandName == "head") {
        if (commandOptions.starts_with('n')) return new HeadCommand();
        throw UnexpectedOptions();
    }

    if (commandName == "prompt") {
        if (commandOptions.empty()) return new PromptCommand();
        throw UnexpectedOptions();
    }

    if (commandName == "rm") {
        if (commandOptions.empty()) return new RemoveCommand();
        throw UnexpectedOptions();
    }

    if (commandName == "time") {
        if (commandOptions.empty()) return new TimeCommand();
        throw UnexpectedOptions();
    }

    if (commandName == "touch") {
        if (commandOptions.empty()) return new TouchCommand();
        throw UnexpectedOptions();
    }

    if (commandName == "tr") {
        if (commandOptions.empty()) return new TrCommand();
        throw UnexpectedOptions();
    }

    if (commandName == "truncate") {
        if (commandOptions.empty()) return new TruncateCommand();
        throw UnexpectedOptions();
    }

    if (commandName == "wc") {
        if (commandOptions == "w" || commandOptions == "c") return new WordCountCommand();
        throw UnexpectedOptions();
    }


    throw UnknownCommand(commandName);
}

unique_ptr<Pipeline> CommandBuilder::getPipeline() const {
    string line = mParser->getLine();
    return buildPipeline(line);
}

unique_ptr<Pipeline> CommandBuilder::buildPipeline(string &line) const {
    auto pipeline = make_unique<Pipeline>(); // const auto pipeline = new Pipeline();

    try {
        stringstream ss(line);

        while (getline(ss, line, '|')) {
            const bool status = pipeline->insidePipeline();
            mParser->setInsidePipeline(status);
            Command *command = assembleCommand(line);
            if (!command) continue;
            pipeline->addCommand(command);
        }

    } catch (const string &e) {
        throw;
    }

    return pipeline;
}