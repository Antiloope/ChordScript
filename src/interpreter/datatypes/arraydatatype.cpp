#include "arraydatatype.h"
#include "interpreter/values/literalvalue.h"
#include <iterator>

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
