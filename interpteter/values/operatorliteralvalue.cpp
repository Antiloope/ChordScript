#include "operatorliteralvalue.h"

using namespace CS;

OperatorLiteralValue::OperatorLiteralValue(char op) :
    LiteralValue(DataTypesId::Operator) {
    _operator = op;
    _value = &_operator;
}

OperatorLiteralValue::~OperatorLiteralValue() {}

LiteralValue* OperatorLiteralValue::clone() {
    return new OperatorLiteralValue(_operator);
}
