#include "stringliteralvalue.h"

using namespace CS;
using namespace std;

StringLiteralValue::StringLiteralValue(string text) :
    LiteralValue(DataTypesId::String) {

    _text = text;
    _value = &_text;
}

StringLiteralValue::~StringLiteralValue() {}

LiteralValue* StringLiteralValue::clone() {
    return new StringLiteralValue(_text);
}
