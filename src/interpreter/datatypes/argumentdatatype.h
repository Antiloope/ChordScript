#ifndef ARGUMENTDATATYPE_H
#define ARGUMENTDATATYPE_H

#include "datatype.h"

namespace CS {

/**
 * @brief This class represents the argument data type.
 * It has not available casts and has two methods: push and pop.
 */
class ArgumentDataType : public DataType
{
public:
    ArgumentDataType();
    ~ArgumentDataType();
    LiteralValue* cast(LiteralValue*) const;
private:
    static LiteralValue* push(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* pop(std::string,const LiteralValue*,const LiteralValue*);
};

}

#endif // ARGUMENTDATATYPE_H
