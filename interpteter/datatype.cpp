#include "datatype.h"
#include "languageConstants.h"

using namespace CS;
using namespace CS::constants;

string DataType::getDataTypeString(DataTypesId dataType) {
    switch (dataType) {
    case DataTypesId::Null: return sNull;
    case DataTypesId::Numeric: return sNumber;
    case DataTypesId::Argument: return sArgument;
    case DataTypesId::Sample: return sSample;
    case DataTypesId::Operator: return "operator";
    case DataTypesId::Array: return "array";
    case DataTypesId::Group: return sGroup;
    case DataTypesId::Boolean: return sBoolean;
    case DataTypesId::Buffer: return sBuffer;
    case DataTypesId::String: return sString;
    case DataTypesId::Function: return sFunction;
    case DataTypesId::Sound: return sSound;
    default: return "unknown";
    }
}

DataTypesId DataType::getDataTypeId(string dataType) {
    if ( !dataType.compare(sNumber) ) return DataTypesId::Numeric;
    if ( !dataType.compare(sArgument) ) return DataTypesId::Argument;
    if ( !dataType.compare(sSample) ) return DataTypesId::Sample;
    if ( !dataType.compare("operator") ) return DataTypesId::Operator;
    if ( !dataType.compare("array") ) return DataTypesId::Array;
    if ( !dataType.compare(sGroup) ) return DataTypesId::Group;
    if ( !dataType.compare(sBoolean) ) return DataTypesId::Boolean;
    if ( !dataType.compare(sBuffer) ) return DataTypesId::Buffer;
    if ( !dataType.compare(sString) ) return DataTypesId::String;
    if ( !dataType.compare(sFunction) ) return DataTypesId::Function;
    if ( !dataType.compare(sSound) ) return DataTypesId::Sound;
    return DataTypesId::Null;
}

#include "value.h"

DataType::DataType() {}

DataType::~DataType() {}

bool DataType::executeMethod(string,LiteralValue*,LiteralValue*) {
    return true;
}

SampleDataType::SampleDataType() {}

SampleDataType::~SampleDataType() {}

LiteralValue* SampleDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Sample:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}

SoundDataType::SoundDataType() {}

SoundDataType::~SoundDataType() {}

LiteralValue* SoundDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Sound:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}

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
        {
            tmp = true;
        }
        else
        {
            tmp = false;
        }
        delete value;
        return new BooleanLiteralValue(tmp);
    }
    default:
        return nullptr;
        break;
    }
}

BufferDataType::BufferDataType() {}

BufferDataType::~BufferDataType() {}

LiteralValue* BufferDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Buffer:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}

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

FunctionDataType::FunctionDataType() {}

FunctionDataType::~FunctionDataType() {}

LiteralValue* FunctionDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Function:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}
