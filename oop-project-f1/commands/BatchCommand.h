//
// Created by lukav on 26.10.2025.
//

#ifndef OOP_PROJECT_F1_BATCHCOMMAND_H
#define OOP_PROJECT_F1_BATCHCOMMAND_H

#include "../Command.h"

class BatchCommand final : public Command {
public:
    BatchCommand();
    string execute() override;
};

inline BatchCommand::BatchCommand() = default;

inline string BatchCommand::execute() {
    if (!mArgs->isContextOnly()) throw UnexpectedArgumentContext();
    return "";
}

#endif //OOP_PROJECT_F1_BATCHCOMMAND_H