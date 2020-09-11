#include "languagedefinitions.h"
#include "context.h"
#include "value.h"
#include "executor/executorinterface.h"
#include <math.h>

#include <string>

using namespace CS;
using namespace std;

double CS::Functions::sin(double x) {
    return std::sin(x);
}

double CS::Functions::square(double x) {
    if( fmod(x,M_PI*2) < M_PI )
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

void CS::Functions::sample() {
    Context* ctx = Context::getInstance();

    LiteralValue* argValue = ctx->getArgumentValue("fileName");
    if( argValue->getDataTypeId() != DataTypesId::String )
    {
        throw new SemanticException("Invalid argument for sample function. Expected a file name");
    }
    string fileName = *(string*)argValue->getValue();

    AudioFile<float> a;
    a.load(fileName);
    ctx->newAudioFile(fileName,a);

    Sample tmp(fileName);
    SampleLiteralValue* ret = new SampleLiteralValue(new SamplePlayer(tmp));

    ctx->setReturnValue(ret);
}

void CS::Functions::stop() {
    Context* ctx = Context::getInstance();

    ExecutorInterface::removeAllSounds();

    SoundDataType soundDataType;
    SampleDataType sampleDataType;
    for( auto&& playable : Context::getInstance()->getAllPlayables() )
    {
        if( get<0>(playable.second) == DataTypesId::Sound )
        {
            soundDataType.executeMethod("stop",(get<1>(playable.second)),new ArgumentLiteralValue({}));
        }
        else
        {
            sampleDataType.executeMethod("stop",(get<1>(playable.second)),new ArgumentLiteralValue({}));
        }
    }

    ctx->setReturnValue(new NullLiteralValue());
}
