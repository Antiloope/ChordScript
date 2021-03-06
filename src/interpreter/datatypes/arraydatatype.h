#ifndef ARRAYDATATYPE_H
#define ARRAYDATATYPE_H

#include "datatype.h"

namespace CS {

/**
 * @brief This class represents the array data type.
 */
class ArrayDataType : public DataType
{
public:
    ArrayDataType();
    ~ArrayDataType();
    LiteralValue* cast(LiteralValue*) const;
private:
    static LiteralValue* size(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* at(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* push(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* insert(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* remove(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* pop(std::string,const LiteralValue*,const LiteralValue*);
};

}

#endif // ARRAYDATATYPE_H
