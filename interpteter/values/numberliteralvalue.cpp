#include "numberliteralvalue.h"

using namespace CS;

NumberLiteralValue::NumberLiteralValue(double number) :
    LiteralValue(DataTypesId::Numeric) {

    _number = number;
    _value = &_number;
}

NumberLiteralValue::~NumberLiteralValue() {}

LiteralValue* NumberLiteralValue::clone() {
    return new NumberLiteralValue(_number);
}
