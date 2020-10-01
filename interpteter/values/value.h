#ifndef VALUE_H
#define VALUE_H

#include "interpteter/datatypes/datatype.h"

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
