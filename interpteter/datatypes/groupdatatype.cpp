#include "groupdatatype.h"
#include "interpteter/value.h"

using namespace CS;

GroupDataType::GroupDataType() {}

GroupDataType::~GroupDataType() {}

LiteralValue* GroupDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Group:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}

