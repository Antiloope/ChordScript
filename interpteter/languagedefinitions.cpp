#include "languagedefinitions.h"
#include "context.h"
#include "value.h"
#include "executor/executorinterface.h"
#include <math.h>

#include <string>

using namespace CS;
using namespace std;

void CS::Functions::sin() {
    Context* ctx = Context::getInstance();

    double freq = 440.f, duration = 1000; // Default values

    LiteralValue* freqLiteralValue = ctx->getArgumentValue(Functions::freqParam,DataTypesId::Numeric);
    if( freqLiteralValue )
    {
        freq = *(double*)freqLiteralValue->getValue();
        delete freqLiteralValue;
    }

    LiteralValue* durationLiteralValue = ctx->getArgumentValue(Functions::durationParam,DataTypesId::Numeric);
    if( durationLiteralValue )
    {
        duration = *(double*)durationLiteralValue->getValue();
        delete durationLiteralValue;
    }

    AudioBuffer buffer = ExecutorInterface::getBuffer(duration);
    unsigned int sampleRate = ExecutorInterface::getSampleRate();

    for( size_t i = 0; i < buffer[0].size(); i++ )
    {
        float sample = sinf (2. * M_PI * ((float) i / sampleRate) * freq) ;

        buffer[0][i] = sample;
        buffer[1][i] = sample;
    }

    BufferLiteralValue* returnValue = new BufferLiteralValue(buffer);

    ctx->setReturnValue(returnValue);
}

void CS::Functions::square() {

}
