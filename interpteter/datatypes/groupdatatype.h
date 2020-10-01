#ifndef GROUPDATATYPE_H
#define GROUPDATATYPE_H

#include "datatype.h"

namespace CS {

class GroupDataType : public DataType
{
public:
    GroupDataType();
    ~GroupDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // GROUPDATATYPE_H
