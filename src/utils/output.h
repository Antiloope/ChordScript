#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>

namespace CS {

class Output
{
public:
    static Output& getInstance();

    void write(const char *);
    void write(std::string);
    void clean();

    void operator<<(const char*);
private:
    inline void _write(const char*);
    Output();
};

}

#endif // OUTPUT_H
