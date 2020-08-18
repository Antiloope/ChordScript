#include "soundgenerator.h"
#include "context.h"

using namespace CS;

SoundGenerator::SoundGenerator(string functionName) : _functionName(functionName) {}

SoundGenerator::SoundGenerator(const SoundGenerator& base) : _functionName(base._functionName), _generatedSounds(base._generatedSounds) {}

SoundGenerator* SoundGenerator::clone() {
    return new SoundGenerator(*this);
}

void SoundGenerator::play(list<double> freqList,double duration,tick_t startTick) {
    Context* ctx = Context::getInstance();
    AudioBuffer returnBuffer(0);

    if( freqList.empty() )
    {
        NumericLiteralValue freqLiteralValue = NumericLiteralValue(0);
        NumericLiteralValue durationLiteralValue = NumericLiteralValue(duration);

        list<LiteralValue*> args;
        args.push_back(&freqLiteralValue);
        args.push_back(&durationLiteralValue);

        ArgumentLiteralValue literalArguments = ArgumentLiteralValue(args);

        if( ctx->executeFunction(this->_functionName,&literalArguments) )
        {
            this->stop();
            throw new SemanticException("Invalid funcition called " + _functionName + " to generate sound");
        }

        LiteralValue* ret = ctx->getReturnValue();
        if( ret->getDataTypeId() != DataTypesId::Buffer )
        {
            this->stop();
            throw new SemanticException("Invalid return value on function " + _functionName);
        }

        AudioBuffer* tmp = ((AudioBuffer*)ret->getValue());
        returnBuffer.resize(2);
        returnBuffer = *tmp;
    }
    else
    {
        for( double freq : freqList )
        {
            NumericLiteralValue* freqLiteralValue = new NumericLiteralValue(freq);
            NumericLiteralValue* durationLiteralValue = new NumericLiteralValue(duration);

            list<LiteralValue*> args;
            args.push_back(freqLiteralValue);
            args.push_back(durationLiteralValue);

            ArgumentLiteralValue literalArguments = ArgumentLiteralValue(args);

            if( !ctx->executeFunction(this->_functionName,&literalArguments) )
            {
                this->stop();
                throw new SemanticException("Invalid funcition called " + _functionName + " to generate sound");
            }

            LiteralValue* ret = ctx->getReturnValue();
            if( ret->getDataTypeId() != DataTypesId::Buffer )
            {
                this->stop();
                throw new SemanticException("Invalid return value on function " + _functionName);
            }

            if( returnBuffer.size() )
            {
                for(size_t i = 0; i < returnBuffer[0].size(); i++ )
                {
                    for( auto j = 0; j < 2; j++ )
                    {
                        AudioBuffer* tmp = ((AudioBuffer*)ret->getValue());
                        returnBuffer[j][i] += tmp->at(j).at(i);
                    }
                }
            }
            else
            {
                AudioBuffer* tmp = ((AudioBuffer*)ret->getValue());
                returnBuffer.resize(2);
                returnBuffer = *tmp;
            }
        }
        for(size_t i = 0; i < returnBuffer.size(); i++ )
        {
            for( auto j = 0; j < 2; j++ )
            {
                returnBuffer[j][i] /= freqList.size();
            }
        }
    }

    Sound* newSound = new Sound(returnBuffer,startTick);
    _generatedSounds.push_back(newSound);

    ExecutorInterface::addSound(newSound);
}

void SoundGenerator::stop() {
    auto i = _generatedSounds.begin();

    while (i != _generatedSounds.end())
    {
        delete (*i);
        _generatedSounds.erase(i);
    }
}
