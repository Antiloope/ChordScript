#ifndef SAMPLELITERALVALUE_H
#define SAMPLELITERALVALUE_H

#include "literalvalue.h"

namespace CS {

class SamplePlayer;

class SampleLiteralValue : public LiteralValue {
public:
    SampleLiteralValue(SamplePlayer*);
    LiteralValue* clone();
    ~SampleLiteralValue();
private:
    SamplePlayer* _samplePlayer;
};

}

#endif // SAMPLELITERALVALUE_H
