#include "executorinterface.h"

#include "executor.h"

using namespace CS;

AudioBuffer ExecutorInterface::getBuffer(double duration) {
    AudioBuffer ret(2);
    ret[0].resize(TimeHandler::getInstance()->msToTicks(duration));
    ret[1].resize(TimeHandler::getInstance()->msToTicks(duration));

    return ret;
}

unsigned int ExecutorInterface::getSampleRate() {
    return Executor::getInstance()->getSampleRate();
}

char ExecutorInterface::getSoundId() {
    return Executor::getInstance()->getSoundId();
}

void ExecutorInterface::addSound(Playable* newSound) {
    Executor::getInstance()->addSound(newSound);
}

void ExecutorInterface::removeSound(Playable* sound) {
    Executor::getInstance()->removeSound(sound);
}

void ExecutorInterface::removeAllSounds() {
    Executor::getInstance()->removeAllSounds();
}

void ExecutorInterface::closeAll() {
    Executor::getInstance()->closeAll();
}
void ExecutorInterface::startRecording() {
    Executor::getInstance()->startRecording();
}

void ExecutorInterface::stopRecording() {
    Executor::getInstance()->stopRecording();
}

int ExecutorInterface::addObserver(function<void(const AudioBuffer&)> callback) {
    return Executor::getInstance()->addObserver(callback);
}

void ExecutorInterface::removeObserver(int index) {
    Executor::getInstance()->removeObserver(index);
}
