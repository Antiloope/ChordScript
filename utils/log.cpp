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

void Log::title(string text) {
    _title(text);
}

void Log::subtitle(string text) {
    _subtitle(text);
}

void Log::close(string text) {
    _close(text);
}

void Log::_write(string text, logType type) {
    ofstream file(LOG_FILE_NAME, ofstream::app);

    time_t t = time( nullptr );

    string time = asctime( localtime(&t));

    time.pop_back();
    time = time.substr(11,8);

    file << "*" << time << "* ";

    switch ( type ) {
    case error_t:
        file << "ERROR ";
        break;
    case warning_t:
        file << "WARNING ";
        break;
    case info_t:
        file << "INFO ";
        break;
    }

    file << "__" << text << "__" << endl << endl;

    file.close();
}

void Log::_title(string text) {
    ofstream file(LOG_FILE_NAME, ofstream::out);

    file << "# " << text << endl << endl;

    file.close();
}

void Log::_subtitle(string text) {
    ofstream file(LOG_FILE_NAME, ofstream::out);

    file << "## " << text << endl << endl;

    file.close();
}

void Log::_close(string text) {
    ofstream file(LOG_FILE_NAME, ofstream::app);

    file << "# " << text << endl << endl << "---" << endl;

    file.close();
}

Log& Log::operator=(const Log&) {
    return *this;
}
