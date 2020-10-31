#include "argumentdatatype.h"
#include "interpreter/values/literalvalue.h"
#include "interpreter/context.h"
#include "utils/Exceptions/exception.h"

using namespace CS;
using namespace std;

namespace  {
const char* POP_METHOD_NAME  = "pop";
const char* PUSH_METHOD_NAME = "push";
}

ArgumentDataType::ArgumentDataType() {
    _methods.insert(
        pair<string,method_function_t>(
            POP_METHOD_NAME,
            &ArgumentDataType::pop)
        );
    _methods.insert(
        pair<string,method_function_t>(
            PUSH_METHOD_NAME,
            &ArgumentDataType::push)
        );
}

ArgumentDataType::~ArgumentDataType() {}

LiteralValue* ArgumentDataType::cast(LiteralValue* value) const {
    switch( value->getDataTypeId() )
    {
    case DataTypesId::Argument:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}

LiteralValue* ArgumentDataType::pop(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in pop method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 0 )
        throw SyntaxException("Pop method must receive no parameters");

    if( !value )
        throw SemanticException("Variable " + variableName + "has not a value");

    auto array = (list<LiteralValue*>*)value->getValue();

    if( !array->empty() )
    {
        delete array->back();
        array->pop_back();
    }

    auto newLiteralValue = new ArgumentLiteralValue(*array);

    Context::getInstance()->setVariableValue(variableName,newLiteralValue);

    return new NullLiteralValue();
}

LiteralValue* ArgumentDataType::push(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in push method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 1 )
        throw SyntaxException("Push method must receive one parameter");

    if( !value )
        throw SemanticException("Variable " + variableName + "has not a value");

    auto array = (list<LiteralValue*>*)value->getValue();

    array->push_back(argumentValues->front()->clone());

    auto newValue = new ArgumentLiteralValue(*array);

    Context::getInstance()->setVariableValue(variableName,newValue);

    return new NullLiteralValue();
}
