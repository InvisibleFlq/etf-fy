//
// Created by lukav on 20.10.2025.
//

#ifndef OOP_PROJECT_F1_PROMPTCOMMAND_H
#define OOP_PROJECT_F1_PROMPTCOMMAND_H
#include "../Command.h"

class PromptCommand final : public Command {
public:
    PromptCommand();
    string execute() override;
};

inline PromptCommand::PromptCommand() = default;

inline string PromptCommand::execute() {
    if (!mArgs->isContextOnly()) throw UnexpectedArgumentContext();
    return "";
}

#endif //OOP_PROJECT_F1_PROMPTCOMMAND_H