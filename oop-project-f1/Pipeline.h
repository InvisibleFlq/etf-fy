//
// Created by lukav on 25.10.2025.
//

#ifndef OOP_PROJECT_F1_PIPELINE_H
#define OOP_PROJECT_F1_PIPELINE_H
#include "Command.h"

#include <vector>


class Pipeline {
public:
    Pipeline();
    ~Pipeline();

    void addCommand(Command *command);
    [[nodiscard]] bool insidePipeline() const;
    vector<Command*> getCommands();


private:
    vector<Command*> mCommands;
};

inline Pipeline::Pipeline() = default;

inline Pipeline::~Pipeline() {
    for (const auto *command : mCommands) {
        delete command;
    }
}

inline void Pipeline::addCommand(Command *command) {
    mCommands.emplace_back(command);
}

inline bool Pipeline::insidePipeline() const {
    return !mCommands.empty();
}

inline vector<Command*> Pipeline::getCommands() {
    return mCommands;
}

#endif //OOP_PROJECT_F1_PIPELINE_H