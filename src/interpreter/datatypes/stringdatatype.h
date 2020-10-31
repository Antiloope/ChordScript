#ifndef STRINGDATATYPE_H
#define STRINGDATATYPE_H

#include "datatype.h"

namespace CS {

/**
 * @brief This class represents the string data type.
 */
class StringDataType : public DataType
{
public:
    StringDataType();
    ~StringDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // STRINGDATATYPE_H
