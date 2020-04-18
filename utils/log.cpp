#include "log.h"
#include <fstream>
#include <ctime>

#define LOG_FILE_NAME "log.txt"

using namespace CS;

Log& Log::getInstance()
{
    static Log instance;
    return instance;
}

Log::Log()
{

}

void Log::write(const char * text, logType type)
{
    std::ofstream file(LOG_FILE_NAME, std::ofstream::app);

    time_t t = time( nullptr );

    std::string time = asctime( localtime(&t));

    time.pop_back();
    time = time.substr(11,8);

    file <<  time;

    switch ( type ) {
        case error_t:
            file << "--ERROR----: ";
            break;
        case warning_t:
            file << "--WARNING--: ";
            break;
        case info_t:
            file << "--INFO-----: ";
            break;
    }

    file << text << std::endl;
}

void Log::write(std::string text, logType type)
{
    std::ofstream file(LOG_FILE_NAME, std::ofstream::app);

    time_t t = time( nullptr );

    std::string time = asctime( localtime(&t));

    time.pop_back();
    time = time.substr(11,8);

    file <<  time;

    switch ( type ) {
        case error_t:
            file << "--ERROR----: ";
            break;
        case warning_t:
            file << "--WARNING--: ";
            break;
        case info_t:
            file << "--INFO-----: ";
            break;
    }

    file << text << std::endl;
}

void Log::write(std::exception e, logType type)
{
    std::ofstream file(LOG_FILE_NAME, std::ofstream::app);

    time_t t = time( nullptr );

    std::string time = asctime( localtime(&t));

    time.pop_back();
    time = time.substr(11,8);

    file <<  time;

    switch ( type ) {
        case error_t:
            file << "--ERROR----: ";
            break;
        case warning_t:
            file << "--WARNING--: ";
            break;
        case info_t:
            file << "--INFO-----: ";
            break;
    }

    file << e.what() << std::endl;
}
Log& Log::operator=(const Log&)
{
    return *this;
}
