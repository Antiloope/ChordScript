#include "nulldatatype.h"
#include "interpreter/values/literalvalue.h"

using namespace CS;

NullDataType::NullDataType() {}

NullDataType::~NullDataType() {}

LiteralValue* NullDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Null:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}
