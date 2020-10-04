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
    static LiteralValue* play(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* stop(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* setPanning(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* constantFreq(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* freqFactor(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* freqModulation(std::string,const LiteralValue*,const LiteralValue*);
    static LiteralValue* ampFactor(std::string,const LiteralValue*,const LiteralValue*);
};

}

#endif // SOUNDDATATYPE_H
