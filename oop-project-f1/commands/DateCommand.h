//
// Created by lukav on 20.10.2025.
//

#ifndef OOP_PROJECT_F1_DATECOMMAND_H
#define OOP_PROJECT_F1_DATECOMMAND_H
#include "../Command.h"

#include <ctime>
#include <iostream>

class DateCommand final : public Command {
public:
    DateCommand();
    string execute() override;
};

inline DateCommand::DateCommand() = default;


inline string DateCommand::execute() {
    if (mArgs->isContext()) throw UnexpectedArgumentContext();
    if (mArgs->isFile()) throw UnexpectedArgumentFilepath();

    char formatted[128];
    const time_t t = time(nullptr);
    const tm *timePtr = localtime(&t);

    strftime(formatted, sizeof(formatted), "%d.%m.%Y.", timePtr);
    return formatted;
}

#endif //OOP_PROJECT_F1_DATECOMMAND_H