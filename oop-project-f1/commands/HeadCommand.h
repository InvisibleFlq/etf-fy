//
// Created by lukav on 24.10.2025.
//

#ifndef OOP_PROJECT_F1_HEADCOMMAND_H
#define OOP_PROJECT_F1_HEADCOMMAND_H
#include "../Command.h"
#include <sstream>
#define LIMIT 99999

class HeadCommand final : public Command {
public:
    HeadCommand();
    string execute() override;
};

inline HeadCommand::HeadCommand() = default;

inline string HeadCommand::execute() {
    if (mOpt == "n") {
        throw UnexpectedOptions();
    }

    string options = mOpt;
    options = options.substr(1);
    const unsigned n = atoi(options.c_str());

    if (n < 0 || n > LIMIT) {
        throw UnexpectedOptionsNumber(n, LIMIT);
    }

    if (!mArgs->isContextOnly()) throw UnexpectedArgumentContext();

    unsigned i = 0;
    stringstream ss(mArgs->getContext());
    string line, output;

    while (getline(ss, line, '\n')) {
        if (i == n) break;
        output += line + '\n';
        i++;
    }

    return output;
}

#endif //OOP_PROJECT_F1_HEADCOMMAND_H