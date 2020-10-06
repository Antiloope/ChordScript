#include "booleandatatype.h"
#include "interpreter/values/literalvalue.h"

using namespace CS;
using namespace std;

BooleanDataType::BooleanDataType() {}

BooleanDataType::~BooleanDataType() {}

LiteralValue* BooleanDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Boolean:
        return value;
        break;
    case DataTypesId::Numeric:
    {
        bool returnValue = *(double*)value->getValue();
        delete value;
        return new BooleanLiteralValue(returnValue);
    }
    case DataTypesId::Null:
    {
        delete value;
        return new BooleanLiteralValue(false);
    }
    case DataTypesId::String:
    {
        string returnValue = *(string*)value->getValue();
        bool tmp;
        if( returnValue.length() )
            tmp = true;
        else
            tmp = false;
        delete value;
        return new BooleanLiteralValue(tmp);
    }
    default:
        return nullptr;
        break;
    }
}
