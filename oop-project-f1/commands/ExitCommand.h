//
// Created by lukav on 20.10.2025.
//

#ifndef OOP_PROJECT_F1_EXITCOMMAND_H
#define OOP_PROJECT_F1_EXITCOMMAND_H

class ExitCommand final : public Command {
public:
    ExitCommand();
    string execute() override;
};

inline ExitCommand::ExitCommand() = default;

inline string ExitCommand::execute() { // nothing, it just exits
    if (mArgs->isContext()) throw UnexpectedArgumentContext();
    if (mArgs->isFile()) throw UnexpectedArgumentFilepath();
    return "";
}

#endif //OOP_PROJECT_F1_EXITCOMMAND_H