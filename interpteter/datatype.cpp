#include "datatype.h"

using namespace CS;

string DataType::getDataTypeString(DataTypesId dataType) {
    switch (dataType) {
    case DataTypesId::Null: return "null";
    case DataTypesId::Numeric: return "numeric";
    case DataTypesId::Argument: return "argument";
    case DataTypesId::Sample: return "sample";
    case DataTypesId::Operator: return "operator";
    case DataTypesId::Array: return "array";
    case DataTypesId::Group: return "group";
    case DataTypesId::Boolean: return "boolean";
    case DataTypesId::Buffer: return "buffer";
    case DataTypesId::String: return "string";
    case DataTypesId::Function: return "function";
    case DataTypesId::Sound: return "sound";
    default: return "unknown";
    }
}

DataTypesId DataType::getDataTypeId(string dataType) {
    if ( !dataType.compare("numeric") ) return DataTypesId::Numeric;
    if ( !dataType.compare("argument") ) return DataTypesId::Argument;
    if ( !dataType.compare("sample") ) return DataTypesId::Sample;
    if ( !dataType.compare("operator") ) return DataTypesId::Operator;
    if ( !dataType.compare("array") ) return DataTypesId::Array;
    if ( !dataType.compare("group") ) return DataTypesId::Group;
    if ( !dataType.compare("boolean") ) return DataTypesId::Boolean;
    if ( !dataType.compare("buffer") ) return DataTypesId::Buffer;
    if ( !dataType.compare("string") ) return DataTypesId::String;
    if ( !dataType.compare("function") ) return DataTypesId::Function;
    if ( !dataType.compare("sound") ) return DataTypesId::Sound;
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
