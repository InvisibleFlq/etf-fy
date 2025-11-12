//
// Created by lukav on 20.10.2025.
//

#ifndef OOP_PROJECT_F1_WORDCOUNTCOMMAND_H
#define OOP_PROJECT_F1_WORDCOUNTCOMMAND_H
#include "../Command.h"

class WordCountCommand final : public Command {
public:
    WordCountCommand();
    string execute() override;
};

inline WordCountCommand::WordCountCommand() = default;

inline string WordCountCommand::execute() {
    if (!mArgs->isContextOnly()) throw UnexpectedArgumentContext();

    if (mOpt == "c") {
        return to_string(mArgs->getContext().length());
    }

    if (mOpt == "w") {
        const string args = mArgs->getContext();
        if (args.empty()) return "0";

        int count = 1;
        for (const char c : args) if (isspace(c)) count++;
        if (isspace(args[0])) count--;
        if (isspace(args[args.length() - 1])) count--;
        return to_string(count);
    }

    return "";
}

#endif //OOP_PROJECT_F1_WORDCOUNTCOMMAND_H