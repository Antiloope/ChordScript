#include "exception.h"

using namespace CS;

Exception::Exception()
{
}

Exception::Exception(const char * _what)
{
    _ = _what;
}

Exception::Exception (const Exception& e)
{
    _ = e.what();
}

const char * Exception::what() const throw()
{
    return _.c_str();
}

Exception::~Exception()
{
}
