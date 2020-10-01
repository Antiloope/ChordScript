#ifndef BOOLEANDATATYPE_H
#define BOOLEANDATATYPE_H

#include "datatype.h"

namespace CS {

class BooleanDataType : public DataType
{
public:
    BooleanDataType();
    ~BooleanDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // BOOLEANDATATYPE_H
