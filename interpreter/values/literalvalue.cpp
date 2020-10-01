#include "literalvalue.h"

using namespace CS;

LiteralValue::LiteralValue(DataTypesId dataType) :
    Value(dataType) {}

LiteralValue::LiteralValue(const LiteralValue& e) :
    Value(e._dataType),
    _value(e._value) {}

LiteralValue::~LiteralValue() {}
