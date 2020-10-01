#ifndef SAMPLEDATATYPE_H
#define SAMPLEDATATYPE_H

#include "datatype.h"

namespace CS {

class SampleDataType : public DataType
{
public:
    SampleDataType();
    ~SampleDataType();
    LiteralValue* cast(LiteralValue*) const;
private:
    static LiteralValue* play(const LiteralValue*,const LiteralValue*);
    static LiteralValue* stop(const LiteralValue*,const LiteralValue*);
    static LiteralValue* setPanning(const LiteralValue*,const LiteralValue*);
};

}

#endif // SAMPLEDATATYPE_H
