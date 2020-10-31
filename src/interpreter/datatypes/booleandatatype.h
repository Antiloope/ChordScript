#ifndef BOOLEANDATATYPE_H
#define BOOLEANDATATYPE_H

#include "datatype.h"

namespace CS {

/**
 * @brief This class represents the boolean data type.
 */
class BooleanDataType : public DataType
{
public:
    BooleanDataType();
    ~BooleanDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // BOOLEANDATATYPE_H
