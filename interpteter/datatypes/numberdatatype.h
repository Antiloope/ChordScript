#ifndef NUMERICDATATYPE_H
#define NUMERICDATATYPE_H

#include "datatype.h"

namespace CS {

class NumberDataType : public DataType
{
public:
    NumberDataType();
    ~NumberDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // NUMERICDATATYPE_H
