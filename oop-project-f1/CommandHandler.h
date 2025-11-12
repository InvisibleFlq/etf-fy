//
// Created by luka on 12/26/24.
//

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include "CommandBuilder.h"
#include "Command.h"

#include <string>

class CommandHandler {
public:
    explicit CommandHandler(string prompt = "$ ");
    ~CommandHandler();

    void start();

protected:
    enum status {EXIT, RUN};

    void printPrompt() const;
    void executePipeline(const unique_ptr<Pipeline> &pipeline);
    void buildPipeline();
    void modifyHandler(Command *command);
    static void handleRedirect(const Command * command, const string &output);

private:
    string mPrompt;
    status mStatus;
    CommandBuilder *mBuilder;
};

#endif //COMMANDHANDLER_H
