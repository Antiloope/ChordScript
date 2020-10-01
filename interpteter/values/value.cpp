#include "value.h"

using namespace CS;

Value::Value(DataTypesId dataType) : _dataType(dataType) {}

Value::~Value() {}

DataTypesId Value::getDataTypeId() const {
    return _dataType;
}
