#ifndef NULLDATATYPE_H
#define NULLDATATYPE_H

#include "datatype.h"

namespace CS {

/**
 * @brief This class represents the null data type.
 * It can't be instanciated in the code but is the value returned by void functions
 */
class NullDataType : public DataType
{
public:
    NullDataType();
    ~NullDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // NULLDATATYPE_H
