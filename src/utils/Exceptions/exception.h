#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
#include "utils/log.h"

namespace CS {

class Exception : public std::exception
{
public:
    explicit Exception();
    explicit Exception( const char * message );
    explicit Exception( const std::string& message );
    explicit Exception ( const Exception& e );
    virtual ~Exception();

    virtual const char * what() const throw();
protected:
    std::string _ = "Base exception thrown";
};

class SyntaxException : public Exception {
public:
    explicit SyntaxException();
    explicit SyntaxException( const std::string& message );
    explicit SyntaxException( const char* message );
    explicit SyntaxException( const std::string& message , size_t ref);
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
    explicit SemanticException( const std::string& message );
    explicit SemanticException( const char* message );
    explicit SemanticException( const std::string& message , size_t ref);
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
    explicit LogException( const std::string& message );
    explicit LogException( const char* message );
    virtual ~LogException();

    virtual const char* what() const throw ();
};

}

#endif // EXCEPTION_H
