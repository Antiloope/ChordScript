#include "exception.h"

using namespace CS;

Exception::Exception() : exception() {}

Exception::Exception(const char * message) : exception(), _(message) {}

Exception::Exception(const string& message) : exception(), _(message) {}

Exception::Exception (const Exception& e) : exception(), _(e.what()) {}

const char * Exception::what() const throw() {
    return _.c_str();
}

Exception::~Exception() {}


SyntaxException::SyntaxException() : Exception() {}

SyntaxException::SyntaxException( const string& message ) : Exception(message) {}

SyntaxException::SyntaxException( const char* message ) : Exception(message) {}

SyntaxException::SyntaxException( const string& message , size_t ref ) :
    Exception(message), _characterRefference(ref) {}

SyntaxException::SyntaxException( const char * message , size_t ref ) :
    Exception(message), _characterRefference(ref) {}
SyntaxException::SyntaxException( const SyntaxException& e ) :
    Exception(e),_characterRefference(e.getCharacterRefference()) {}
SyntaxException::~SyntaxException() {}

const char* SyntaxException::what() const throw() {
    return _.c_str();
}

size_t SyntaxException::getCharacterRefference() const {
    return _characterRefference;
}

LogException::LogException() : Exception() {}

LogException::LogException(const char * message) : Exception(message){
    Log::getInstance().write(message,Log::error_t);
}

LogException::LogException(const string& message) : Exception(message) {
    Log::getInstance().write(message,Log::error_t);
}

const char * LogException::what() const throw() {
    return _.c_str();
}

LogException::~LogException() {}
