#include "argumentdatatype.h"
#include "interpreter/values/literalvalue.h"

using namespace CS;

ArgumentDataType::ArgumentDataType() {}

ArgumentDataType::~ArgumentDataType() {}

LiteralValue* ArgumentDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Argument:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}
