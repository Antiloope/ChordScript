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

DataType::DataType()
{

}
