#include "arraydatatype.h"
#include "interpreter/values/literalvalue.h"
#include "interpreter/context.h"

using namespace CS;
using namespace std;

ArrayDataType::ArrayDataType() {
    _methods.insert(
        pair<string,method_function_t>(
            "size",
            &ArrayDataType::size)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "at",
            &ArrayDataType::at)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "append",
            &ArrayDataType::append)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "insert",
            &ArrayDataType::insert)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "remove",
            &ArrayDataType::remove)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "pop",
            &ArrayDataType::pop)
        );
}

ArrayDataType::~ArrayDataType() {}

LiteralValue* ArrayDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Array:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}

LiteralValue* ArrayDataType::size(
    string,
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

    return new NumberLiteralValue(array->size());
}

LiteralValue* ArrayDataType::at(
    string,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        return nullptr;

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 1 )
        return nullptr;

    if( argumentValues->front()->getDataTypeId() != DataTypesId::Numeric )
        return nullptr;

    unsigned index = *(double*)argumentValues->front()->getValue();

    if( !value )
        return nullptr;

    auto array = (list<LiteralValue*>*)value->getValue();

    if( array->size() < index )
        return nullptr;

    auto it = array->begin();
    for(unsigned i = 0; i < index; i++)
        it++;

    return (*it)->clone();
}

LiteralValue* ArrayDataType::append(
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

    auto newValue = new ArrayLiteralValue(*array);

    Context::getInstance()->setVariableValue(variableName,newValue);

    return new NullLiteralValue();
}

LiteralValue* ArrayDataType::insert(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        return nullptr;

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 2 )
        return nullptr;

    if( argumentValues->front()->getDataTypeId() != DataTypesId::Numeric )
        return nullptr;

    unsigned index = *(double*)argumentValues->front()->getValue();
    argumentValues->pop_front();
    LiteralValue* newValue = argumentValues->front();

    if( !value )
        return nullptr;

    auto array = (list<LiteralValue*>*)value->getValue();

    auto it = array->begin();
    for(unsigned i = 0; i < index; i++)
        it++;

    array->insert(it,newValue->clone());

    auto newLiteralValue = new ArrayLiteralValue(*array);

    Context::getInstance()->setVariableValue(variableName,newLiteralValue);

    return new NullLiteralValue();
}

LiteralValue* ArrayDataType::remove(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        return nullptr;

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 1 )
        return nullptr;

    if( argumentValues->front()->getDataTypeId() != DataTypesId::Numeric )
        return nullptr;

    unsigned index = *(double*)argumentValues->front()->getValue();

    if( !value )
        return nullptr;

    auto array = (list<LiteralValue*>*)value->getValue();

    auto it = array->begin();
    for(unsigned i = 0; i < index; i++)
        it++;

    delete *it;
    array->erase(it);

    auto newLiteralValue = new ArrayLiteralValue(*array);

    Context::getInstance()->setVariableValue(variableName,newLiteralValue);

    return new NullLiteralValue();
}

LiteralValue* ArrayDataType::pop(
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

    delete array->back();
    array->pop_back();

    auto newLiteralValue = new ArrayLiteralValue(*array);

    Context::getInstance()->setVariableValue(variableName,newLiteralValue);

    return new NullLiteralValue();
}
