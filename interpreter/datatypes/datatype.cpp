#include "datatype.h"
#include "interpteter/context.h"
#include "interpteter/value.h"

using namespace CS;
using namespace CS::Constants;
using namespace std;

string DataType::getDataTypeString(DataTypesId dataType) {
    switch (dataType) {
    case DataTypesId::Null: return S_NULL;
    case DataTypesId::Numeric: return NUMBER;
    case DataTypesId::Argument: return ARGUMENT;
    case DataTypesId::Sample: return SAMPLE;
    case DataTypesId::Operator: return "operator";
    case DataTypesId::Array: return "array";
    case DataTypesId::Group: return GROUP;
    case DataTypesId::Boolean: return BOOLEAN;
    case DataTypesId::String: return STRING;
    case DataTypesId::Sound: return SOUND;
    default: return "unknown";
    }
}

DataTypesId DataType::getDataTypeId(string dataType) {
    if ( !dataType.compare(NUMBER) ) return DataTypesId::Numeric;
    if ( !dataType.compare(ARGUMENT) ) return DataTypesId::Argument;
    if ( !dataType.compare(SAMPLE) ) return DataTypesId::Sample;
    if ( !dataType.compare("operator") ) return DataTypesId::Operator;
    if ( !dataType.compare("array") ) return DataTypesId::Array;
    if ( !dataType.compare(GROUP) ) return DataTypesId::Group;
    if ( !dataType.compare(BOOLEAN) ) return DataTypesId::Boolean;
    if ( !dataType.compare(STRING) ) return DataTypesId::String;
    if ( !dataType.compare(SOUND) ) return DataTypesId::Sound;
    return DataTypesId::Null;
}

DataType::DataType() {}

DataType::~DataType() {}

bool DataType::executeMethod(
    string methodName,
    const LiteralValue* value,
    const LiteralValue* arguments
    ) {

    if( _methods.find(methodName) == _methods.end() )
        return false;

    auto method = _methods.find(methodName)->second;

    LiteralValue* returnValue = method(value,arguments);
    if( !returnValue )
        return false;

    Context::getInstance()->setReturnValue(returnValue);

    delete returnValue;

    return true;
}
