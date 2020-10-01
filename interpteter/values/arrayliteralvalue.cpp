#include "arrayliteralvalue.h"

using namespace CS;
using namespace std;

ArrayLiteralValue::ArrayLiteralValue(list<LiteralValue*> literalValuesList) :
    LiteralValue(DataTypesId::Array) {
    for( auto literalValue : literalValuesList )
        _literalValuesList.push_back(literalValue->clone());

    _value = &_literalValuesList;
}

ArrayLiteralValue::~ArrayLiteralValue() {
    while( !_literalValuesList.empty() )
    {
        if( _literalValuesList.front() )
            delete _literalValuesList.front();

        _literalValuesList.pop_front();
    }
}

LiteralValue* ArrayLiteralValue::clone() {
    return new ArrayLiteralValue(_literalValuesList);
}
