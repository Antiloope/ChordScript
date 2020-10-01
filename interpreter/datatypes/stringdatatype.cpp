#include "stringdatatype.h"
#include "interpreter/values/literalvalue.h"

using namespace CS;

StringDataType::StringDataType() {}

StringDataType::~StringDataType() {}

LiteralValue* StringDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::String:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}
