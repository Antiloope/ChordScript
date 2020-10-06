#include "executorinterface.h"
#include "executor.h"
#include "utils/log.h"

using namespace CS;
using namespace std;

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

void ExecutorInterface::addSound(Playable* newSound,string variableName) {
    Executor::getInstance()->addSound(newSound,variableName);
}

void ExecutorInterface::removeSound(string variableName) {
    Executor::getInstance()->removeSound(nullptr,variableName);
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

void ExecutorInterface::startServer() {
    if( !Executor::getInstance()->isServerRunning() )
        Executor::getInstance()->startServer();

    Log::getInstance().write("Server was running",Log::logType::info_t);
}

void ExecutorInterface::restartServer() {
    if( Executor::getInstance()->isServerRunning() )
        Executor::getInstance()->restartServer();
    else
        Executor::getInstance()->startServer();
}

void ExecutorInterface::killServer() {
    if( Executor::getInstance()->isServerRunning() )
        Executor::getInstance()->killServer();
}
