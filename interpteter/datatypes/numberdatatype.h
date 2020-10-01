#ifndef NUMERICDATATYPE_H
#define NUMERICDATATYPE_H

#include "datatype.h"

namespace CS {

class NumericDataType : public DataType
{
public:
    NumericDataType();
    ~NumericDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // NUMERICDATATYPE_H
