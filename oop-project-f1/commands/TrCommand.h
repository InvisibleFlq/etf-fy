//
// Created by lukav on 26.10.2025.
//

#ifndef OOP_PROJECT_F1_TRCOMMAND_H
#define OOP_PROJECT_F1_TRCOMMAND_H

class TrCommand final : public Command {
public:
    TrCommand();
    string execute() override;
};

inline TrCommand::TrCommand() = default;

inline string TrCommand::execute() {
    if (!mArgs->isContextOnly()) throw UnexpectedArgumentContext();
    string ctx = mArgs->getContext();
    const string lookup = mArgs->getWhat();
    if (lookup.empty()) throw UnexpectedArgument("missing lookup");
    const string replace = mArgs->getWith();

    while (true) {
        const int index = static_cast<int>(ctx.find(lookup));
        if (index == string::npos) break;
        ctx.replace(index, lookup.length(), replace);
    }

    return ctx;
}

#endif //OOP_PROJECT_F1_TRCOMMAND_H