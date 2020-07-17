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

SampleDataType::SampleDataType() {}

SampleDataType::~SampleDataType() {}

bool SampleDataType::executeMethod(string,LiteralValue*,LiteralValue*) {}

LiteralValue* SampleDataType::cast(LiteralValue *) const {}

SoundDataType::SoundDataType() {}

SoundDataType::~SoundDataType() {}

bool SoundDataType::executeMethod(string,LiteralValue*,LiteralValue*) {}

LiteralValue* SoundDataType::cast(LiteralValue*) const {}

NumericDataType::NumericDataType() {}

NumericDataType::~NumericDataType() {}

bool NumericDataType::executeMethod(string,LiteralValue*,LiteralValue*) {}

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

bool ArgumentDataType::executeMethod(string,LiteralValue*,LiteralValue*) {}

LiteralValue* ArgumentDataType::cast(LiteralValue *) const {}

GroupDataType::GroupDataType() {}

GroupDataType::~GroupDataType() {}

bool GroupDataType::executeMethod(string,LiteralValue*,LiteralValue*) {}

LiteralValue* GroupDataType::cast(LiteralValue *) const {}

BooleanDataType::BooleanDataType() {}

BooleanDataType::~BooleanDataType() {}

bool BooleanDataType::executeMethod(string,LiteralValue*,LiteralValue*) {}

LiteralValue* BooleanDataType::cast(LiteralValue *) const {}

BufferDataType::BufferDataType() {}

BufferDataType::~BufferDataType() {}

bool BufferDataType::executeMethod(string,LiteralValue*,LiteralValue*) {}

LiteralValue* BufferDataType::cast(LiteralValue *) const {}

StringDataType::StringDataType() {}

StringDataType::~StringDataType() {}

bool StringDataType::executeMethod(string,LiteralValue*,LiteralValue*) {}

LiteralValue* StringDataType::cast(LiteralValue *) const {}

NullDataType::NullDataType() {}

NullDataType::~NullDataType() {}

bool NullDataType::executeMethod(string,LiteralValue*,LiteralValue*) {}

LiteralValue* NullDataType::cast(LiteralValue *) const {}

FunctionDataType::FunctionDataType() {}

FunctionDataType::~FunctionDataType() {}

bool FunctionDataType::executeMethod(string,LiteralValue*,LiteralValue*) {}

LiteralValue* FunctionDataType::cast(LiteralValue *) const {}
