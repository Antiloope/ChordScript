#include "numberdatatype.h"
#include "interpteter/value.h"

using namespace CS;

NumericDataType::NumericDataType() {}

NumericDataType::~NumericDataType() {}

LiteralValue* NumericDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Numeric:
        return value;
        break;
    case DataTypesId::Boolean:
    {
        int returnValue = *(bool*)value->getValue();
        delete value;
        return new NumericLiteralValue(returnValue);
    }
    break;
    default:
        return nullptr;
        break;
    }
}
