#include "output.h"
#include <fstream>

using namespace CS;
using namespace std;

const char * OUTPUT_FILE_NAME = "output.txt";

Output& Output::getInstance() {
    static Output instance;
    return instance;
}

void Output::operator<<(const char* text) {
    _write(text);
}

void Output::write(const char * text) {
    _write(text);
}

void Output::write(std::string text) {
    _write(text.c_str());
}

void Output::clean() {
    ofstream file(OUTPUT_FILE_NAME, ofstream::trunc);
    file << endl;
    file.close();
}

void Output::_write(const char* text) {
    ofstream file(OUTPUT_FILE_NAME, ofstream::trunc);

    file << text << endl << endl;

    file.close();
}

Output::Output() {}
