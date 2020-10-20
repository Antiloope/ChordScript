#include "argumentdatatype.h"
#include "interpreter/values/literalvalue.h"
#include "interpreter/context.h"

using namespace CS;
using namespace std;

ArgumentDataType::ArgumentDataType() {
    _methods.insert(
        pair<string,method_function_t>(
            "pop",
            &ArgumentDataType::pop)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "push",
            &ArgumentDataType::push)
        );
}

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

LiteralValue* ArgumentDataType::pop(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        return nullptr;

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 0 )
        return nullptr;

    if( !value )
        return nullptr;

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
        return nullptr;

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 1 )
        return nullptr;

    if( !value )
        return nullptr;

    auto array = (list<LiteralValue*>*)value->getValue();

    array->push_back(argumentValues->front()->clone());

    auto newValue = new ArgumentLiteralValue(*array);

    Context::getInstance()->setVariableValue(variableName,newValue);

    return new NullLiteralValue();
}
