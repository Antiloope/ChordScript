#include "sampledatatype.h"
#include "interpreter/context.h"
#include "interpreter/values/literalvalue.h"
#include "interpreter/players/sampleplayer.h"
#include "interpreter/players/soundgenerator.h"
#include "utils/Exceptions/exception.h"

using namespace CS;
using namespace CS::Constants;
using namespace std;

SampleDataType::SampleDataType() {
    _methods.insert(
        pair<string,method_function_t>(
            "play",
            &SampleDataType::play)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "stop",
            &SampleDataType::stop)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "setPanning",
            &SampleDataType::setPanning)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "loop",
            &SampleDataType::loop)
        );
}

SampleDataType::~SampleDataType() {}

LiteralValue* SampleDataType::stop(
    std::string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in stop method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !argumentValues->empty() )
        throw SyntaxException("Stop method must receive no parameters");

    if( !value || value->getDataTypeId() != DataTypesId::Sample)
        throw SemanticException("Variable " + variableName + " has not a value");

    SamplePlayer* player = (SamplePlayer*)value->getValue();

    player->stop(variableName);

    return new NullLiteralValue();
}

LiteralValue* SampleDataType::play(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in play method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value || value->getDataTypeId() != DataTypesId::Sample )
        throw SemanticException("Variable " + variableName + " has not a value");

    SamplePlayer* player = (SamplePlayer*)value->getValue();

    tick_t startTick = TimeHandler::getInstance()->getCurrentTick();

    if( argumentValues->empty() )
    {
        player->play(1,startTick,variableName);
        return new NullLiteralValue();
    }

    auto it = argumentValues->begin();

    switch( (*it)->getDataTypeId() )
    {
    case DataTypesId::Numeric:
    {
        double timeFactor = *(double*)((NumberLiteralValue*)(*it))->getValue();

        it++;
        if( it == argumentValues->end() )
        {
            player->play(timeFactor,startTick,variableName);
            return new NullLiteralValue();
        }
        return nullptr;
        break;
    }
    case DataTypesId::Argument:
    {
        double duration = player->getDurationInSeconds();
        if( ((list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue())->front()->getDataTypeId() == DataTypesId::Argument )
        {
            argumentValues = (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();
            it = argumentValues->begin();
        }
        for( ; it != argumentValues->end(); it++ )
        {
            list<LiteralValue*>* argumentList =
                (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();

            auto argIt = argumentList->begin();

            if( (*argIt) == nullptr || (*argIt)->getDataTypeId() != DataTypesId::Numeric )
                throw SyntaxException("Invalid argument contents to play");

            double timeFactor = *(double*)(*argIt)->getValue();

            argIt++;
            if( argIt == argumentList->end() )
            {
                player->play(timeFactor, startTick, variableName);
                startTick += TimeHandler::getInstance()->segToTicks(duration);
                continue;
            }

            if( (*argIt) == nullptr || (*argIt)->getDataTypeId() != DataTypesId::Numeric )
                throw SyntaxException("Invalid argument contents to play");

            double virtualDuration = *(double*)(*argIt)->getValue();
            argIt++;
            if( argIt != argumentList->end() )
                throw SyntaxException("Invalid number of arguments in play method");
            if( virtualDuration == 0 )
            {
                startTick += TimeHandler::getInstance()->segToTicks(duration * timeFactor);
                continue;
            }
            player->play(timeFactor, startTick, variableName);
            startTick += TimeHandler::getInstance()->segToTicks(virtualDuration);
        }
        break;
    }
    default:
        return nullptr;
        break;
    }

    return new NullLiteralValue();
}

LiteralValue* SampleDataType::loop(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in loop method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value || value->getDataTypeId() != DataTypesId::Sample )
        throw SemanticException("Variable " + variableName + " has not a value");

    SamplePlayer* player = (SamplePlayer*)value->getValue();

    tick_t startTick = TimeHandler::getInstance()->getCurrentTick();

    if( argumentValues->empty() )
    {
        player->loop(1,startTick,variableName);
        return new NullLiteralValue();
    }

    auto it = argumentValues->begin();

    switch( (*it)->getDataTypeId() )
    {
    case DataTypesId::Numeric:
    {
        double timeFactor = *(double*)((NumberLiteralValue*)(*it))->getValue();
        if( timeFactor <= 0 )
            throw SemanticException("Invalid time factor value. It must be possitive");

        it++;
        if( it == argumentValues->end() )
        {
            player->loop(timeFactor,startTick,variableName);
            return new NullLiteralValue();
        }
        throw SemanticException("Invalid number of arguments in loop method");
        break;
    }
    case DataTypesId::Argument:
    {
        double duration = player->getDurationInSeconds();
        double totalLoopDuration = 0;
        if( ((list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue())->front()->getDataTypeId() == DataTypesId::Argument )
        {
            argumentValues = (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();
            it = argumentValues->begin();
        }
        for( ; it != argumentValues->end(); it++ )
        {
            list<LiteralValue*>* argumentList =
                (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();

            auto argIt = argumentList->begin();

            if( (*argIt) == nullptr || (*argIt)->getDataTypeId() != DataTypesId::Numeric )
                throw SyntaxException("Invalid argument contents to play");

            argIt++;
            if( argIt == argumentList->end() )
            {
                totalLoopDuration += duration;
                continue;
            }

            if( (*argIt) == nullptr || (*argIt)->getDataTypeId() != DataTypesId::Numeric )
                throw SyntaxException("Invalid argument contents to play");

            double virtualDuration = *(double*)(*argIt)->getValue();
            argIt++;
            if( argIt != argumentList->end() )
                throw SemanticException("Invalid number of arguments in loop method");

            if( virtualDuration == 0 )
                totalLoopDuration += duration;
            else
                totalLoopDuration += virtualDuration;
        }
        auto it = argumentValues->begin();
        for( ; it != argumentValues->end(); it++ )
        {
            list<LiteralValue*>* argumentList =
                (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();

            auto argIt = argumentList->begin();

            double timeFactor = *(double*)(*argIt)->getValue();

            argIt++;
            if( argIt == argumentList->end() )
            {
                player->loop(timeFactor, totalLoopDuration, startTick, variableName);
                startTick += TimeHandler::getInstance()->segToTicks(duration);
                continue;
            }

            double virtualDuration = *(double*)(*argIt)->getValue();
            argIt++;

            player->loop(timeFactor, totalLoopDuration, startTick, variableName);

            startTick += TimeHandler::getInstance()->segToTicks(virtualDuration);
        }

        break;
    }
    default:
        return nullptr;
        break;
    }

    return new NullLiteralValue();
}

LiteralValue* SampleDataType::setPanning(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in setPanning method");

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value || value->getDataTypeId() != DataTypesId::Sample )
        throw SemanticException("Variable " + variableName + " has not a value");

    SamplePlayer* player = (SamplePlayer*)value->getValue();

    if( argumentValues->empty() )
        throw SemanticException("Invalid call of method setPanning without arguments.");

    if( argumentValues->size() > 1 )
        throw SemanticException("Invalid call of method setPanning more than one parameter.");

    switch( argumentValues->front()->getDataTypeId() )
    {
    case DataTypesId::Sound:
    {
        Sound panningValue = ((SoundGenerator*)argumentValues->front()->getValue())->getSound();
        player->_sample.setPanning(panningValue);
        break;
    }
    case DataTypesId::Numeric:
    {
        double panningValue = *(double*)argumentValues->front()->getValue();
        player->_sample.setPanning(panningValue);
        break;
    }
    default:
        throw SemanticException("Invalid cast parameter data type in setPanning method. Must be a number or a sound.");
        break;
    }

    return new NullLiteralValue();
}

LiteralValue* SampleDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Sample:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}
