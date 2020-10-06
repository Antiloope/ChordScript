#ifndef STRINGDATATYPE_H
#define STRINGDATATYPE_H

#include "datatype.h"

namespace CS {

class StringDataType : public DataType
{
public:
    StringDataType();
    ~StringDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // STRINGDATATYPE_H
