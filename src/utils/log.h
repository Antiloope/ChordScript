#ifndef LOG_H
#define LOG_H

#include <string>

namespace CS {

class Log
{
public:
    static Log& getInstance();

    enum logType{
        error_t = 0,
        warning_t,
        info_t
    };

    void write(const char *, logType);
    void write(std::string, logType);
    void write(std::exception, logType);

    Log& operator=(const Log&);
private:
    inline void _write(std::string, logType);
    Log();
};

}

#endif // LOG_H
