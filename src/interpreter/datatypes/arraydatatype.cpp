#include "arraydatatype.h"
#include "interpreter/values/literalvalue.h"
#include "interpreter/context.h"
#include "utils/Exceptions/exception.h"

using namespace CS;
using namespace std;

namespace {
const char* SIZE_METHOD_NAME = "size";
const char* AT_METHOD_NAME = "at";
const char* PUSH_METHOD_NAME = "push";
const char* INSERT_METHOD_NAME = "insert";
const char* REMOVE_METHOD_NAME = "remove";
const char* POP_METHOD_NAME = "pop";
}

/**
 * @brief This class represents the array data type.
 */
ArrayDataType::ArrayDataType() {
    _methods.insert(
        pair<string,method_function_t>(
            SIZE_METHOD_NAME,
            &ArrayDataType::size)
        );
    _methods.insert(
        pair<string,method_function_t>(
            AT_METHOD_NAME,
            &ArrayDataType::at)
        );
    _methods.insert(
        pair<string,method_function_t>(
            PUSH_METHOD_NAME,
            &ArrayDataType::push)
        );
    _methods.insert(
        pair<string,method_function_t>(
            INSERT_METHOD_NAME,
            &ArrayDataType::insert)
        );
    _methods.insert(
        pair<string,method_function_t>(
            REMOVE_METHOD_NAME,
            &ArrayDataType::remove)
        );
    _methods.insert(
        pair<string,method_function_t>(
            POP_METHOD_NAME,
            &ArrayDataType::pop)
        );
}

ArrayDataType::~ArrayDataType() {}

LiteralValue* ArrayDataType::cast(LiteralValue* value) const {
    switch( value->getDataTypeId() )
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
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in size method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 0 )
        throw SyntaxException("Size method must receive no parameters");

    if( !value || value->getDataTypeId() != DataTypesId::Array )
        throw SemanticException("Variable " + variableName + "has not a value");

    auto array = (list<LiteralValue*>*)value->getValue();

    return new NumberLiteralValue(array->size());
}

LiteralValue* ArrayDataType::at(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in 'at' method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 1 )
        throw SyntaxException("Method 'at' must receive one parameter");

    if( argumentValues->front()->getDataTypeId() != DataTypesId::Numeric )
        throw SyntaxException("Method 'at' must receive a number as paramter");

    unsigned index = *(double*)argumentValues->front()->getValue();

    if( !value || value->getDataTypeId() != DataTypesId::Array )
        throw SemanticException("Variable " + variableName + "has not a value");

    auto array = (list<LiteralValue*>*)value->getValue();

    if( array->size() < index )
        throw SemanticException("Index out of range");;

    auto it = array->begin();
    for(unsigned i = 0; i < index; i++)
        it++;

    return (*it)->clone();
}

LiteralValue* ArrayDataType::push(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in push method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 1 )
        throw SyntaxException("Push method must receive one parameter");

    if( !value || value->getDataTypeId() != DataTypesId::Array )
        throw SemanticException("Variable " + variableName + "has not a value");

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
        throw SyntaxException("Invalid argument at insert method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 2 )
        throw SyntaxException("Insert method must receive two parameters. Position and value");

    if( argumentValues->front()->getDataTypeId() != DataTypesId::Numeric )
        throw SemanticException("First argument must be a number");

    unsigned index = *(double*)argumentValues->front()->getValue();
    argumentValues->pop_front();
    LiteralValue* newValue = argumentValues->front();

    if( !value || value->getDataTypeId() != DataTypesId::Array )
        throw SemanticException("Variable " + variableName + "has not a value");

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
        throw SyntaxException("Invalid argument in remove method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 1 )
        throw SyntaxException("Pop method must receive one parameter");

    if( argumentValues->front()->getDataTypeId() != DataTypesId::Numeric )
        throw SemanticException("First argument must be a number in remove method");

    unsigned index = *(double*)argumentValues->front()->getValue();

    if( !value || value->getDataTypeId() != DataTypesId::Array )
        throw SemanticException("Variable " + variableName + "has not a value");

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
        throw SyntaxException("Invalid argument in pop method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( argumentValues->size() != 0 )
        throw SyntaxException("Pop method must receive no parameters");

    if( !value || value->getDataTypeId() != DataTypesId::Array )
        throw SemanticException("Variable " + variableName + "has not a value");

    auto array = (list<LiteralValue*>*)value->getValue();

    delete array->back();
    array->pop_back();

    auto newLiteralValue = new ArrayLiteralValue(*array);

    Context::getInstance()->setVariableValue(variableName,newLiteralValue);

    return new NullLiteralValue();
}
