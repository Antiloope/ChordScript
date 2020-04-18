#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

namespace CS {

class Exception : public std::exception
{
public:
    Exception();
    Exception(const char * _what);
    Exception (const Exception&);
    ~Exception();

    virtual const char * what() const throw();
private:
    std::string _ = "Base exception thrown";
};

}

#endif // EXCEPTION_H
