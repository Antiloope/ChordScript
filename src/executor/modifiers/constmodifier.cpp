#include "constmodifier.h"
#include "executor/playables/sound.h"

using namespace CS;

ConstModifier::ConstModifier(double value) {
    _value = value;
}

ConstModifier::~ConstModifier() {}

double ConstModifier::getValue(double) {
    return _value;
}

double ConstModifier::getPositiveValue(double) {
    if( _value >= 0 )
        return _value;
    return - _value;
}

bool ConstModifier::setAmplitudeOffset(Sound) {
    return false;
}

bool ConstModifier::setAmplitudeFactor(Sound) {
    return false;
}

Modifier* ConstModifier::clone() {
    return new ConstModifier(_value);
}
