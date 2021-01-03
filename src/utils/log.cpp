#include "log.h"
#include <fstream>
#include <ctime>
#include <iostream>
#include <sys/stat.h>

using namespace CS;
using namespace std;

const char* LOG_FILE_NAME = "cs.log";
const char* LOG_FOLDER_PATH = "/tmp/cs/";

string Log::getLogFilePath()
{
    return string(LOG_FOLDER_PATH) + string(LOG_FILE_NAME);
}

Log& Log::getInstance() {
    static Log instance;
    return instance;
}

Log::Log() {
    struct stat info;
    stat( LOG_FOLDER_PATH, &info );

    if(info.st_mode & S_IFDIR)
        return;

    mkdir(LOG_FOLDER_PATH,0777);
}

void Log::write(const char * text, LogType type) {
    _write(text,type);
}

void Log::write(string text, LogType type) {
    _write(text,type);
}

void Log::write(exception e, LogType type) {
    _write(e.what(),type);
}

void Log::_write(string text, LogType type) {
    ofstream file(string(LOG_FOLDER_PATH) + string(LOG_FILE_NAME), ofstream::app);

    time_t t = time( nullptr );

    string time = asctime( localtime(&t));

    time.pop_back();
    time = time.substr(11,8);

    file << time;

    switch ( type ) {
    case error_t:
        file << ": ERROR ";
        break;
    case warning_t:
        file << ": WARNING ";
        break;
    case info_t:
        file << ": ";
        break;
    }

    file << text << endl;

    file.close();
}
