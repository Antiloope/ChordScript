#ifndef LOG_H
#define LOG_H

#include <string>

namespace CS {

class Log
{
public:
    static Log& getInstance();

    enum LogType {
        error_t = 0,
        warning_t,
        info_t
    };

    void write(const char *, LogType);
    void write(std::string, LogType);
    void write(std::exception, LogType);
    std::string getLogFilePath();
private:
    inline void _write(std::string, LogType);
    Log();
    std::ofstream* _file;
};

}

#endif // LOG_H
