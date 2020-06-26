#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
#include "utils/log.h"

using namespace std;

namespace CS {

class Exception : public exception
{
public:
    explicit Exception();
    explicit Exception( const char * message );
    explicit Exception( const string& message );
    explicit Exception ( const Exception& e );
    virtual ~Exception();

    virtual const char * what() const throw();
protected:
    string _ = "Base exception thrown";
};

class SyntaxException : public Exception {
public:
    explicit SyntaxException();
    explicit SyntaxException( const string& message );
    explicit SyntaxException( const char* message );
    explicit SyntaxException( const string& message , size_t ref);
    explicit SyntaxException( const char* message , size_t ref);
    SyntaxException ( const SyntaxException& e );
    virtual ~SyntaxException();

    virtual const char* what() const throw ();

    virtual size_t getCharacterRefference() const;
protected:
    size_t _characterRefference = 0;
};

class SemanticException : public Exception {
public:
    explicit SemanticException();
    explicit SemanticException( const string& message );
    explicit SemanticException( const char* message );
    explicit SemanticException( const string& message , size_t ref);
    explicit SemanticException( const char* message , size_t ref);
    SemanticException ( const SemanticException& e );
    virtual ~SemanticException();

    virtual const char* what() const throw ();

    virtual size_t getCharacterRefference() const;
protected:
    size_t _characterRefference = 0;
};

class LogException : public Exception {
public:
    explicit LogException();
    explicit LogException( const string& message );
    explicit LogException( const char* message );
    virtual ~LogException();

    virtual const char* what() const throw ();
};

}

#endif // EXCEPTION_H
