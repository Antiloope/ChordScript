#include "numberdatatype.h"
#include "interpreter/values/literalvalue.h"

using namespace CS;

NumberDataType::NumberDataType() {}

NumberDataType::~NumberDataType() {}

LiteralValue* NumberDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Numeric:
        return value;
        break;
    case DataTypesId::Boolean:
    {
        int returnValue = *(bool*)value->getValue();
        delete value;
        return new NumberLiteralValue(returnValue);
    }
    break;
    default:
        return nullptr;
        break;
    }
}
