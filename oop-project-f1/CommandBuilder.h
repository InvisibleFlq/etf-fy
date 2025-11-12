//
// Created by lukav on 20.10.2025.
//

#ifndef OOP_PROJECT_F1_COMMANDBUILDER_H
#define OOP_PROJECT_F1_COMMANDBUILDER_H
#include "Parser.h"
#include "Command.h"
#include "Pipeline.h"

#include <memory>

class CommandBuilder {
public:
    explicit CommandBuilder(Parser *parser = new Parser());
    ~CommandBuilder();

    [[nodiscard]] Command *getCommand(const string &line) const;
    [[nodiscard]] unique_ptr<Pipeline> getPipeline() const;

protected:
    [[nodiscard]] Command *assembleCommand(const string &line) const;

    static Command *buildCommand(const string &commandName, const string &commandOptions);

    unique_ptr<Pipeline> buildPipeline(string &line) const;

private:
    Parser *mParser;

};

#endif //OOP_PROJECT_F1_COMMANDBUILDER_H