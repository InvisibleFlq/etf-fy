//
// Created by luka on 12/26/24.
//

#include "Reader.h"
#include <iostream>

using namespace std;

Reader::Reader(istream *input) : mInput(input) {}

Reader::~Reader() = default;

string Reader::getNextLine() {
    string line;
    getline(*mInput, line);
    return line;
}

bool Reader::endOfRead() {
    return mInput->eof();
}

bool Reader::checkFile() {
    return mInput->good();
}

// console reader
ConsoleReader::ConsoleReader() : Reader(&cin) {}

string ConsoleReader::getNextLine() {
    string line = Reader::getNextLine();
    if (line.length() > 511) line = line.substr(0, 511);
    return line;
}

// file reader
FileReader::FileReader(const string& filepath) : Reader(new ifstream(filepath)) {}

FileReader::~FileReader() {
    ((ifstream*) mInput)->close();
    delete mInput;
}
