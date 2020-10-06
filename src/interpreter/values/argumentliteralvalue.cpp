#include "argumentliteralvalue.h"

using namespace CS;
using namespace std;

ArgumentLiteralValue::ArgumentLiteralValue(list<LiteralValue*> literalValuesList) :
    LiteralValue(DataTypesId::Argument) {
    for( auto literalValue : literalValuesList )
        _literalValuesList.push_back(literalValue->clone());

    _value = &_literalValuesList;
}

ArgumentLiteralValue::~ArgumentLiteralValue() {
    while ( !_literalValuesList.empty() ) {
        if(_literalValuesList.front())
            delete _literalValuesList.front();

        _literalValuesList.pop_front();
    }
}

LiteralValue* ArgumentLiteralValue::clone() {
    return new ArgumentLiteralValue(_literalValuesList);
}
