//
// Created by lukav on 20.10.2025.
//

#ifndef OOP_PROJECT_F1_TIMECOMMAND_H
#define OOP_PROJECT_F1_TIMECOMMAND_H
#include "../Command.h"
#include <ctime>

class TimeCommand final : public Command {
public:
    TimeCommand();
    string execute() override;
};

inline TimeCommand::TimeCommand() = default;

inline string TimeCommand::execute() {
    if (mArgs->isContext()) throw UnexpectedArgumentContext();
    if (mArgs->isFile()) throw UnexpectedArgumentFilepath();

    char formatted[128];
    const time_t t = time(nullptr);
    const tm *timePtr = localtime(&t);

    strftime(formatted, sizeof(formatted), "%H:%M:%S", timePtr);
    return formatted;
}

#endif //OOP_PROJECT_F1_TIMECOMMAND_H