#ifndef SOUNDLITERALVALUE_H
#define SOUNDLITERALVALUE_H

#include "literalvalue.h"

namespace CS {

class SoundGenerator;

class SoundLiteralValue : public LiteralValue {
public:
    SoundLiteralValue(SoundGenerator*);
    LiteralValue* clone();
    ~SoundLiteralValue();
private:
    SoundGenerator* _soundGenerator;
};

}


#endif // SOUNDLITERALVALUE_H
