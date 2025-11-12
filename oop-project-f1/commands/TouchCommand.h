//
// Created by lukav on 20.10.2025.
//

#ifndef OOP_PROJECT_F1_TOUCHCOMMAND_H
#define OOP_PROJECT_F1_TOUCHCOMMAND_H
#include "../Command.h"
#include "../Exceptions.h"

#include <fstream>

class TouchCommand final : public Command {
public:
    TouchCommand();
    string execute() override;
};

inline TouchCommand::TouchCommand() = default;

inline string TouchCommand::execute() {
    if (!mArgs->isFileOnly()) throw UnexpectedArgumentFilepath();

    ifstream file;
    string filepath = mArgs->getFilepath();

    file.open(filepath);
    if (file.good()) {
        file.close();
        throw FileAlreadyExists(filepath);
    }

    ofstream newFile;
    newFile.open(filepath);
    if (!newFile.is_open()) {
        throw CouldNotOpenFile(filepath);
    }

    newFile << ".";
    newFile.close();
    return "";
}

#endif //OOP_PROJECT_F1_TOUCHCOMMAND_H