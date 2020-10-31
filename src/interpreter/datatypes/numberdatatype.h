#ifndef NUMERICDATATYPE_H
#define NUMERICDATATYPE_H

#include "datatype.h"

namespace CS {

/**
 * @brief This class represents the number data type.
 */
class NumberDataType : public DataType
{
public:
    NumberDataType();
    ~NumberDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // NUMERICDATATYPE_H
