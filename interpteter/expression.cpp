#include "expression.h"
#include "utils/Exceptions/exception.h"
#include "context.h"

using namespace CS;

Expression::Expression() : _codeReference(0) {

}
Expression::Expression(size_t codeReference) : _codeReference(codeReference) {

}
Expression::~Expression(){

}
size_t Expression::getCodeReference() {
    return _codeReference;
}
