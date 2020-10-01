#include "sampleliteralvalue.h"
#include "interpreter/aux/sampleplayer.h"

using namespace CS;

SampleLiteralValue::SampleLiteralValue(SamplePlayer* samplePlayer) :
    LiteralValue(DataTypesId::Sample) {
    _samplePlayer = samplePlayer;
    _value = samplePlayer;
}

LiteralValue* SampleLiteralValue::clone() {
    return new SampleLiteralValue(_samplePlayer->clone());
}

SampleLiteralValue::~SampleLiteralValue() {
    if( _samplePlayer ) delete _samplePlayer;
}
