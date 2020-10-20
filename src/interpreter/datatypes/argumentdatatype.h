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
private:
    static LiteralValue* push(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* pop(std::string,const LiteralValue*,const LiteralValue*);
};

}

#endif // ARGUMENTDATATYPE_H
