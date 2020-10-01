#ifndef ARGUMENTDATATYPE_H
#define ARGUMENTDATATYPE_H

#include "datatype.h"

namespace CS {

class ArgumentDataType : public DataType
{
public:
    ArgumentDataType();
    ~ArgumentDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // ARGUMENTDATATYPE_H
