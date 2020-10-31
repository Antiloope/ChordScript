#include "expression.h"

using namespace CS;

Expression::Expression() : _codeReference(0) {}

Expression::Expression(size_t codeReference) :
    _codeReference(codeReference) {}

Expression::~Expression(){}

size_t Expression::getCodeReference() const {
    return _codeReference;
}
