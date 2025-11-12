//
// Created by luka on 12/26/24.
//

#include "CommandHandler.h"
#include "CommandBuilder.h"
#include "Exceptions.h"
#include "Pipeline.h"
#include "Command.h"

#include <iostream>
#include <sstream>
#include <utility>
#include <string>

using namespace std;

CommandHandler::CommandHandler(string prompt) : mPrompt(std::move(prompt)), mStatus(RUN) {
    mBuilder = new CommandBuilder();
}

CommandHandler::~CommandHandler() {
    delete mBuilder;
}

void CommandHandler::printPrompt() const {
    cout << mPrompt;
}

void CommandHandler::start() {
    while (mStatus == RUN) {
        printPrompt();
        buildPipeline();
    }
}

void CommandHandler::buildPipeline() {
    try {
        const unique_ptr<Pipeline> pipeline = mBuilder->getPipeline();
        executePipeline(pipeline);
    }
    catch (const exception &e) {
        cout << e.what() << endl;
    }
}

void CommandHandler::executePipeline(const unique_ptr<Pipeline> &pipeline) {
    try {
        const vector<Command*> commands = pipeline->getCommands();
        string output;

        for (auto *command : commands) {
            if (!command) continue;
            if (!output.empty()) {
                CommandArgs *args = command->getArgs();
                args->setContext(output);
            }
            output = command->execute();

            if (command->getArgs()->isRedirected()) {
                handleRedirect(command, output);
                output = "";
            }

            modifyHandler(command); // for commands which change the handler
        }

        if (!output.empty()) cout << output << endl;
    }
    catch (const exception &e) {
        cout << e.what() << endl;
    }
}

void CommandHandler::modifyHandler(Command *command) { // effects of cmds which modify commandHandler
    if (command->getCmd() == "exit") mStatus = EXIT;

    if (command->getCmd() == "prompt") {
        mPrompt = command->getArgs()->getContext();
    }

    if (command->getCmd() == "batch") {
        stringstream ss(command->getArgs()->getContext());
        string line;

        while (getline(ss, line, '\n')) {
            Command *newCommand = mBuilder->getCommand(line);
            auto pipeline = make_unique<Pipeline>(); // auto *pipeline = new Pipeline();
            pipeline->addCommand(newCommand);
            executePipeline(pipeline);
        }
    }
}

void CommandHandler::handleRedirect(const Command *command, const string &output) {
    fstream file;
    const string filepath = command->getArgs()->getRedirectPath();

    if (command->getArgs()->isAppend()) file.open(filepath, ios::app);
    else file.open(filepath, ios::out | ios::trunc);

    if (!file.good() || !file.is_open()) {
        file.close();
        throw CouldNotOpenFile(filepath);
    }

    file << output;

    file.close();

}