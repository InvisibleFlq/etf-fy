//
// Created by lukav on 24.10.2025.
//

#ifndef OOP_PROJECT_F1_TRUNCATECOMMAND_H
#define OOP_PROJECT_F1_TRUNCATECOMMAND_H
#include "../Command.h"
#include "../Exceptions.h"

#include <iostream>
#include <fstream>

class TruncateCommand final : public Command {
public:
    TruncateCommand();
    string execute() override;
};

inline TruncateCommand::TruncateCommand() = default;

inline string TruncateCommand::execute() {
    if (!mArgs->isFileOnly()) throw UnexpectedArgumentFilepath();
    ifstream file;
    const string filepath = mArgs->getFilepath();

    file.open(filepath, ofstream::out | ofstream::trunc);
    if (!file.good() || !file.is_open()) {
        throw CouldNotOpenFile(filepath);
    }

    file.close();
    return "";
}

#endif //OOP_PROJECT_F1_TRUNCATECOMMAND_H