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

    void title(std::string);
    void subtitle(std::string);
    void close(std::string);

    Log& operator=(const Log&);
private:
    void _write(std::string, logType);
    void _title(std::string);
    void _subtitle(std::string);
    void _close(std::string);
    Log();
};

}

#endif // LOG_H
