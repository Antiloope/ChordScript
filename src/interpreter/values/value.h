#ifndef VALUE_H
#define VALUE_H

#include "interpreter/datatypes/datatype.h"

namespace CS {

class Value
{
public:
    Value(DataTypesId);
    ~Value();
    DataTypesId getDataTypeId() const;
protected:
    DataTypesId _dataType;
};

}

#endif // VALUE_H
