//
// Created by lukav on 24.10.2025.
//

#ifndef OOP_PROJECT_F1_EXCEPTIONS_H
#define OOP_PROJECT_F1_EXCEPTIONS_H

#include <stdexcept>
#include <string>

using namespace std;

// base exception
class CLIException : public runtime_error {
public:
    explicit CLIException(const string &msg) : runtime_error(msg) {}
};

class UnknownCommand final : public CLIException {
public:
    explicit UnknownCommand(const string &cmdName) : CLIException("Error - unknown command " + cmdName) {}
};

class UnexpectedOptions final : public CLIException {
public:
    explicit UnexpectedOptions() : CLIException("Error - unexpected option") {}
};

class UnexpectedOptionsNumber final : public CLIException {
public:
    explicit UnexpectedOptionsNumber(const unsigned number, const unsigned limit) : CLIException("Error - option number too large (" + std::to_string(number) + " > " + std::to_string(limit) + ")") {}
};

// base argument exception
class UnexpectedArgument : public CLIException {
public:
    explicit UnexpectedArgument(const string &msg) : CLIException("Error - unexpected argument " + msg) {}
};

class UnexpectedArgumentContext final : public UnexpectedArgument {
public:
    explicit UnexpectedArgumentContext() : UnexpectedArgument("context") {}
};

class UnexpectedArgumentFilepath final : public UnexpectedArgument {
public:
    explicit UnexpectedArgumentFilepath() : UnexpectedArgument("filepath") {}
};

// other exceptions
class UnexpectedCharacters final : public CLIException {
public:
    explicit UnexpectedCharacters(const string &msg) : CLIException(msg) {}
};

class CouldNotOpenFile final : public CLIException {
public:
    explicit CouldNotOpenFile(const string &filepath) : CLIException("Error - could not open file " + filepath) {}
};

class FileAlreadyExists final : public CLIException {
public:
    explicit FileAlreadyExists(const string &filepath) : CLIException("Error - file " + filepath + " already exists") {}
};

#endif //OOP_PROJECT_F1_EXCEPTIONS_H