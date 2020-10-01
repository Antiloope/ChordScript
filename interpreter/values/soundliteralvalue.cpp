#include "soundliteralvalue.h"
#include "interpreter/aux/soundgenerator.h"

using namespace CS;

SoundLiteralValue::SoundLiteralValue(SoundGenerator* generator) :
    LiteralValue(DataTypesId::Sound) {
    _soundGenerator = generator;
    _value = _soundGenerator;
}

LiteralValue* SoundLiteralValue::clone() {
    return new SoundLiteralValue(_soundGenerator->clone());
}

SoundLiteralValue::~SoundLiteralValue() {
    if( _soundGenerator )
        delete _soundGenerator;
}
