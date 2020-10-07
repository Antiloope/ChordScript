#include "soundmodifier.h"

using namespace CS;

SoundModifier::SoundModifier(Sound sound) : _sound(sound) {}

SoundModifier::~SoundModifier() {}

double SoundModifier::getValue(double freq) {
    return _sound.getInstantValue(freq);
}

double SoundModifier::getPositiveValue(double freq) {
    return _sound.getPositiveInstantValue(freq);
}

bool SoundModifier::setAmplitudeOffset(Sound sound) {
    _sound.setAmplitudeOffset(sound);
    return true;
}

bool SoundModifier::setAmplitudeFactor(Sound sound) {
    _sound.setAmplitudeFactor(sound);
    return true;
}

Modifier* SoundModifier::clone() {
    return new SoundModifier(_sound);
}
