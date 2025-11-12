//
// Created by lukav on 20.10.2025.
//

#ifndef OOP_PROJECT_F1_ECHOCOMMAND_H
#define OOP_PROJECT_F1_ECHOCOMMAND_H
#include "../Command.h"

// echo command
class EchoCommand final : public Command {
public:
    EchoCommand();
    string execute() override;
};

inline EchoCommand::EchoCommand() = default;

inline string EchoCommand::execute() {
    return mArgs->getContext();
}

#endif //OOP_PROJECT_F1_ECHOCOMMAND_H