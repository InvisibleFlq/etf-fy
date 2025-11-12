//
// Created by lukav on 24.10.2025.
//

#ifndef OOP_PROJECT_F1_REMOVECOMMAND_H
#define OOP_PROJECT_F1_REMOVECOMMAND_H
#include "../Command.h"

class RemoveCommand final : public Command {
public:
    RemoveCommand();
    string execute() override;
};

inline RemoveCommand::RemoveCommand() = default;

inline string RemoveCommand::execute() {
    if (!mArgs->isFileOnly()) throw UnexpectedArgumentFilepath();
    std::remove(mArgs->getFilepath().c_str());
    return "";
}

#endif //OOP_PROJECT_F1_REMOVECOMMAND_H