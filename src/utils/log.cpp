#include "log.h"
#include <fstream>
#include <ctime>

using namespace CS;
using namespace std;

const string LOG_FILE_NAME = "log.md";

Log& Log::getInstance() {
    static Log instance;
    return instance;
}

Log::Log() {
    remove(LOG_FILE_NAME.c_str());
}

void Log::write(const char * text, logType type) {
    _write(text,type);
}

void Log::write(string text, logType type) {
    _write(text,type);
}

void Log::write(exception e, logType type) {
    _write(e.what(),type);
}

void Log::_write(string text, logType type) {
    ofstream file(LOG_FILE_NAME, ofstream::app);

    time_t t = time( nullptr );

    string time = asctime( localtime(&t));

    time.pop_back();
    time = time.substr(11,8);

    file << "**" << time;

    switch ( type ) {
    case error_t:
        file << " ERROR** ";
        break;
    case warning_t:
        file << " WARNING** ";
        break;
    case info_t:
        file << " INFO** ";
        break;
    }

    file << text << endl << endl;

    file.close();
}

Log& Log::operator=(const Log&) {
    return *this;
}
