#include "nullliteralvalue.h"

using namespace CS;

NullLiteralValue::NullLiteralValue() :
    LiteralValue(DataTypesId::Null) {
    _value = nullptr;
}

NullLiteralValue::~NullLiteralValue() {}

LiteralValue* NullLiteralValue::clone() {
    return new NullLiteralValue();
}
