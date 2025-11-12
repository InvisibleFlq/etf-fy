//
// Created by luka on 12/26/24.
//

#ifndef READER_H
#define READER_H

#include <fstream>
#include <string>

using namespace std;

class Reader {
public:
    explicit Reader(istream *input);
    virtual ~Reader();

    virtual string getNextLine();
    virtual bool endOfRead();
    virtual bool checkFile();

protected:
    istream *mInput;
};

class ConsoleReader final : public Reader {
public:
    ConsoleReader();
    string getNextLine() override;
};

class FileReader final : public Reader {
public:
    explicit FileReader(const string& filepath);
    ~FileReader() override;
};

#endif //READER_H
