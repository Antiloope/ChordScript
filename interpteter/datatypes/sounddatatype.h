#ifndef SOUNDDATATYPE_H
#define SOUNDDATATYPE_H

#include "datatype.h"

namespace CS {

class SoundDataType : public DataType
{
public:
    SoundDataType();
    ~SoundDataType();
    LiteralValue* cast(LiteralValue*) const;
private:
    static LiteralValue* play(const LiteralValue*,const LiteralValue*);
    static LiteralValue* stop(const LiteralValue*,const LiteralValue*);
    static LiteralValue* setPanning(const LiteralValue*,const LiteralValue*);
    static LiteralValue* constantFreq(const LiteralValue*,const LiteralValue*);
    static LiteralValue* freqFactor(const LiteralValue*,const LiteralValue*);
    static LiteralValue* freqModulation(const LiteralValue*,const LiteralValue*);
    static LiteralValue* ampFactor(const LiteralValue*,const LiteralValue*);
};

}

#endif // SOUNDDATATYPE_H
