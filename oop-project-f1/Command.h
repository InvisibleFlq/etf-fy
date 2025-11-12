//
// Created by luka on 12/26/24.
//

#ifndef COMMANDS_H
#define COMMANDS_H
#include "Exceptions.h" // used in every command for error handling (DO NOT REMOVE!)

#include <string>
#include <utility>

using namespace std;

class CommandArgs {
public:
    string getContext();
    void setContext(string args);

    string getFilepath();
    void setFilepath(string filepath);

    string getWhat();
    void setWhat(string what);

    string getWith();
    void setWith(string with);

    void redirectOutput(string path, bool append = false);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] bool isContext() const;
    [[nodiscard]] bool isFile() const;
    [[nodiscard]] bool isContextOnly() const;
    [[nodiscard]] bool isFileOnly() const;

    [[nodiscard]] bool isRedirected() const;
    [[nodiscard]] bool isAppend() const;
    [[nodiscard]] string getRedirectPath() const;

private:
    string mContext;
    string mFilepath;
    bool outputAppend = false;
    string outputPath;

    string mWhat;
    string mWith;
};

inline string CommandArgs::getContext() {
    return mContext;
}

inline void CommandArgs::setContext(string args) {
    mContext = std::move(args);
}

inline string CommandArgs::getFilepath() {
    return mFilepath;
}

inline void CommandArgs::setFilepath(string filepath) {
    mFilepath = std::move(filepath);
}

inline string CommandArgs::getWhat() {
    return mWhat;
}

inline void CommandArgs::setWhat(string what) {
    mWhat = std::move(what);
}

inline string CommandArgs::getWith() {
    return mWith;
}

inline void CommandArgs::setWith(string with) {
    mWith = std::move(with);
}

inline bool CommandArgs::isEmpty() const {
    return mContext.empty() && mFilepath.empty();
}

inline bool CommandArgs::isContext() const {
    return !mContext.empty();
}

inline bool CommandArgs::isFile() const {
    return !mFilepath.empty();
}

inline bool CommandArgs::isContextOnly() const {
    return !isFile() && isContext();
}

inline bool CommandArgs::isFileOnly() const {
    return isFile() && !isContext();
}

inline void CommandArgs::redirectOutput(string path, const bool append) {
    outputAppend = append;
    outputPath = std::move(path);
}

inline bool CommandArgs::isRedirected() const {
    return !outputPath.empty();
}

inline bool CommandArgs::isAppend() const {
    return outputAppend;
}

inline string CommandArgs::getRedirectPath() const {
    return outputPath;
}

// command template

class Command {
public:
    explicit Command(string options = "", CommandArgs *argument = nullptr);
    virtual ~Command();

    void setOpt(string opt);
    void setArgs(CommandArgs *args);
    void setCmd(string cmd);

    string getCmd();
    [[nodiscard]] CommandArgs *getArgs() const;
    virtual string execute() = 0;

protected:
    string mCmd;
    string mOpt;
    CommandArgs *mArgs;
};

inline void Command::setCmd(string cmd) {
    mCmd = std::move(cmd);
}

inline void Command::setOpt(string opt) {
    mOpt = std::move(opt);
}

inline void Command::setArgs(CommandArgs *args) {
    mArgs = args;
}

inline string Command::getCmd() {
    return mCmd;
}

inline CommandArgs *Command::getArgs() const {
    return mArgs;
}

inline Command::Command(string options, CommandArgs *argument) : mOpt(std::move(options)), mArgs(argument) {}

inline Command::~Command() {
    delete mArgs;
}

#endif //COMMANDS_H
