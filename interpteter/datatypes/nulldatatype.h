#ifndef NULLDATATYPE_H
#define NULLDATATYPE_H

#include "datatype.h"

namespace CS {

class NullDataType : public DataType
{
public:
    NullDataType();
    ~NullDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // NULLDATATYPE_H
