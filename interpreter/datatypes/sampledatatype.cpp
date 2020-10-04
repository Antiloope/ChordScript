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
}

SampleDataType::~SampleDataType() {}

LiteralValue* SampleDataType::stop(
    std::string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        return nullptr;

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !argumentValues->empty() )
        return nullptr;

    if( !value )
        return nullptr;

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
        return nullptr;

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        return nullptr;

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
        for( ; it != argumentValues->end(); it++ )
        {
            list<LiteralValue*>* argumentList =
                (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();

            auto argIt = argumentList->begin();

            if( (*argIt)->getDataTypeId() != DataTypesId::Numeric )
            {
                player->stop(variableName);
                return nullptr;
            }

            double timeFactor = *(double*)(*argIt)->getValue();

            argIt++;
            if( argIt == argumentList->end() )
            {
                player->play(timeFactor, startTick, variableName);
                startTick += TimeHandler::getInstance()->segToTicks(duration);
                continue;
            }

            if( (*argIt)->getDataTypeId() != DataTypesId::Numeric )
            {
                player->stop(variableName);
                return nullptr;
            }

            double preFactor = *(double*)(*argIt)->getValue();
            argIt++;
            if( argIt != argumentList->end() )
            {
                player->stop(variableName);
                return nullptr;
            }
            if( preFactor == 0 )
            {
                startTick += TimeHandler::getInstance()->segToTicks(timeFactor);
                continue;
            }
            player->play(timeFactor, startTick, variableName);
            startTick += TimeHandler::getInstance()->segToTicks(duration*preFactor);
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
    string,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        return nullptr;

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

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
