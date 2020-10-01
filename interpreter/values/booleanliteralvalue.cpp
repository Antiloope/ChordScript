#include "booleanliteralvalue.h"

using namespace CS;

BooleanLiteralValue::BooleanLiteralValue(bool boolean) :
    LiteralValue(DataTypesId::Boolean) {
    _boolean = boolean;
    _value = &_boolean;
}

BooleanLiteralValue::~BooleanLiteralValue() {}

LiteralValue* BooleanLiteralValue::clone() {
    return new BooleanLiteralValue(_boolean);
}
