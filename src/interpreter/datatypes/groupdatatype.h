#ifndef GROUPDATATYPE_H
#define GROUPDATATYPE_H

#include "datatype.h"

namespace CS {

/**
 * @brief This class represents the group data type.
 */
class GroupDataType : public DataType
{
public:
    GroupDataType();
    ~GroupDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // GROUPDATATYPE_H
