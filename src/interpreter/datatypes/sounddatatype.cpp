#include "sounddatatype.h"
#include "interpreter/context.h"
#include "interpreter/values/literalvalue.h"
#include <memory>
#include "interpreter/players/soundgenerator.h"
#include "utils/Exceptions/exception.h"

using namespace CS;
using namespace CS::Constants;
using namespace std;

namespace  {
const char* PLAY_METHOD_NAME                = "play";
const char* LOOP_METHOD_NAME                = "loop";
const char* STOP_METHOD_NAME                = "stop";
const char* SET_PANNING_METHOD_NAME         = "setPanning";
const char* CONSTANT_FREQ_METHOD_NAME       = "constantFreq";
const char* FREQ_FACTOR_METHOD_NAME         = "freqFactor";
const char* FREQ_MODULATION_METHOD_NAME     = "freqModulation";
const char* AMP_FACTOR_METHOD_NAME          = "ampFactor";
const char* AMP_MODULATION_METHOD_NAME      = "ampModulation";

unsigned PROCESSING_DELAY_CORRECTION = 5000;
}

SoundDataType::SoundDataType() {
    _methods.insert(
        pair<string,method_function_t>(
            PLAY_METHOD_NAME,
            &SoundDataType::play)
        );
    _methods.insert(
        pair<string,method_function_t>(
            LOOP_METHOD_NAME,
            &SoundDataType::loop)
        );
    _methods.insert(
        pair<string,method_function_t>(
            STOP_METHOD_NAME,
            &SoundDataType::stop)
        );
    _methods.insert(
        pair<string,method_function_t>(
            SET_PANNING_METHOD_NAME,
            &SoundDataType::setPanning)
        );
    _methods.insert(
        pair<string,method_function_t>(
            CONSTANT_FREQ_METHOD_NAME,
            &SoundDataType::constantFreq)
        );
    _methods.insert(
        pair<string,method_function_t>(
            FREQ_FACTOR_METHOD_NAME,
            &SoundDataType::freqFactor)
        );
    _methods.insert(
        pair<string,method_function_t>(
            FREQ_MODULATION_METHOD_NAME,
            &SoundDataType::freqModulation)
        );
    _methods.insert(
        pair<string,method_function_t>(
            AMP_FACTOR_METHOD_NAME,
            &SoundDataType::ampFactor)
        );
    _methods.insert(
        pair<string,method_function_t>(
            AMP_MODULATION_METHOD_NAME,
            &SoundDataType::ampModulation)
        );
}

SoundDataType::~SoundDataType() {}

LiteralValue* SoundDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Sound:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}

LiteralValue* SoundDataType::stop(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in stop method");

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !argumentValues->empty() )
        throw SyntaxException("Stop method must receive no parameters");

    if( !value )
        throw SemanticException("Variable " + variableName + "has not a value");

    SoundGenerator* generator = (SoundGenerator*)value->getValue();

    generator->stop(variableName);

    return new NullLiteralValue();
}

LiteralValue* SoundDataType::play(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in play method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        throw SemanticException("Variable " + variableName + "has not a value");

    SoundGenerator* generator = (SoundGenerator*)value->getValue();

    tick_t startTick = TimeHandler::getInstance()->getCurrentTick() + PROCESSING_DELAY_CORRECTION;

    if( argumentValues->empty() )
    {
        generator->play({},0,startTick,variableName);
        return new NullLiteralValue();
    }

    auto it = argumentValues->begin();

    switch( (*it)->getDataTypeId() )
    {
    case DataTypesId::Array:
    {
        const list<LiteralValue*>* arrayValues = (list<LiteralValue*>*)((ArrayLiteralValue*)(*it))->getValue();
        list<double> freqList;

        for(auto arrayIt = arrayValues->begin(); arrayIt != arrayValues->end(); arrayIt++)
        {
            if( (*arrayIt)->getDataTypeId() != DataTypesId::Numeric )
                throw SemanticException("Invalid notes to play");

            freqList.push_back(*(double*)(*arrayIt)->getValue());
        }
        it++;
        if( it == argumentValues->end() )
        {
            generator->play(freqList,0,startTick,variableName);
            return new NullLiteralValue();
        }

        if( (*it)->getDataTypeId() != DataTypesId::Numeric )
            throw SemanticException("Invalid duration in play method");

        double duration = *(double*)(*it)->getValue();
        it++;

        if( it != argumentValues->end() )
            throw SyntaxException("Invalid syntax inside arguments of play method, after duration. Expected no more parameters");

        generator->play(freqList,duration,startTick,variableName);
        break;
    }
    case DataTypesId::Argument:
    {
        if( ((list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue())->front()->getDataTypeId() == DataTypesId::Argument )
        {
            argumentValues = (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();
            it = argumentValues->begin();
        }
        for( ; it != argumentValues->end(); it++ )
        {
            const list<LiteralValue*>* argumentList = (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();
            auto argIt = argumentList->begin();

            if( (*argIt)->getDataTypeId() != DataTypesId::Array )
                throw SyntaxException("Invalid argument contents to play");

            const list<LiteralValue*>* arrayValues = (list<LiteralValue*>*)((ArrayLiteralValue*)(*argIt))->getValue();
            list<double> freqList;

            for( auto arrayIt = arrayValues->begin(); arrayIt != arrayValues->end(); arrayIt++ )
            {
                if( (*arrayIt)->getDataTypeId() != DataTypesId::Numeric )
                    throw SyntaxException("Invalid notes to play");

                freqList.push_back(*(double*)(*arrayIt)->getValue());
            }
            argIt++;
            if( argIt == argumentList->end() )
            {
                generator->play(freqList,0,startTick,variableName);
                return new NullLiteralValue();
            }

            if( (*argIt)->getDataTypeId() != DataTypesId::Numeric )
                throw SemanticException("Invalid duration in play method");

            double duration = *(double*)(*argIt)->getValue();
            argIt++;

            if( argIt != argumentList->end() )
                throw SyntaxException("Invalid syntax inside arguments of play method, after duration. Expected no more parameters");


            generator->play(freqList,duration,startTick,variableName);
            startTick += TimeHandler::getInstance()->segToTicks(duration);
        }
        break;
    }
    default:
        return nullptr;
        break;
    }

    return new NullLiteralValue();
}

LiteralValue* SoundDataType::loop(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in loop method");

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        throw SemanticException("Variable " + variableName + "has not a value");

    SoundGenerator* generator = (SoundGenerator*)value->getValue();

    tick_t startTick = TimeHandler::getInstance()->getCurrentTick();

    if( argumentValues->empty() )
    {
        generator->play({},0,startTick,variableName);
        return new NullLiteralValue();
    }

    auto it = argumentValues->begin();

    switch( (*it)->getDataTypeId() )
    {
    case DataTypesId::Array:
    {
        const list<LiteralValue*>* arrayValues = (list<LiteralValue*>*)((ArrayLiteralValue*)(*it))->getValue();
        list<double> freqList;

        for(auto arrayIt = arrayValues->begin(); arrayIt != arrayValues->end(); arrayIt++)
        {
            if( (*arrayIt)->getDataTypeId() != DataTypesId::Numeric )
                throw SemanticException("Invalid notes to play");

            freqList.push_back(*(double*)(*arrayIt)->getValue());
        }
        it++;
        if( it == argumentValues->end() )
        {
            generator->play(freqList,0,startTick,variableName);
            return new NullLiteralValue();
        }

        if( (*it)->getDataTypeId() != DataTypesId::Numeric )
            throw SemanticException("Invalid duration in loop method");

        double duration = *(double*)(*it)->getValue();
        it++;

        if( it != argumentValues->end() )
            throw SyntaxException("Invalid syntax inside arguments of loop method, after duration. Expected no more parameters");

        generator->loop(freqList,duration,duration,startTick,variableName);
        break;
    }
    case DataTypesId::Argument:
    {
        if( ((list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue())->front()->getDataTypeId() == DataTypesId::Argument )
        {
            argumentValues = (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();
            it = argumentValues->begin();
        }
        double totalLoopDuration = 0;
        for( ; it != argumentValues->end(); it++ )
        {
            const list<LiteralValue*>* argumentList = (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();
            auto argIt = argumentList->begin();

            if( (*argIt)->getDataTypeId() != DataTypesId::Array )
                throw SyntaxException("Invalid argument contents to play in loop");

            const list<LiteralValue*>* arrayValues = (list<LiteralValue*>*)((ArrayLiteralValue*)(*argIt))->getValue();
            list<double> freqList;

            for( auto arrayIt = arrayValues->begin(); arrayIt != arrayValues->end(); arrayIt++ )
            {
                if( (*arrayIt)->getDataTypeId() != DataTypesId::Numeric )
                    throw SyntaxException("Invalid notes to play in loop");
                freqList.push_back(*(double*)(*arrayIt)->getValue());
            }
            argIt++;
            if( argIt == argumentList->end() )
                throw SemanticException("Invalid duration in loop method");

            if( (*argIt)->getDataTypeId() != DataTypesId::Numeric )
                throw SemanticException("Invalid duration in loop method");

            totalLoopDuration += *(double*)(*argIt)->getValue();
            argIt++;

            if( argIt != argumentList->end() )
                throw SyntaxException("Invalid syntax inside arguments of loop method, after duration. Expected no more parameters");
        }
        auto it = argumentValues->begin();
        for( ; it != argumentValues->end(); it++ )
        {
            const list<LiteralValue*>* argumentList = (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();
            auto argIt = argumentList->begin();

            const list<LiteralValue*>* arrayValues = (list<LiteralValue*>*)((ArrayLiteralValue*)(*argIt))->getValue();
            list<double> freqList;

            for( auto arrayIt = arrayValues->begin(); arrayIt != arrayValues->end(); arrayIt++ )
                freqList.push_back(*(double*)(*arrayIt)->getValue());

            argIt++;

            double duration = *(double*)(*argIt)->getValue();
            argIt++;

            generator->loop(freqList,duration,totalLoopDuration,startTick,variableName);
            startTick += TimeHandler::getInstance()->segToTicks(duration);
        }
        break;
    }
    default:
        return nullptr;
        break;
    }

    return new NullLiteralValue();
}

LiteralValue* SoundDataType::setPanning(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in setPanning method");

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        throw SemanticException("Variable " + variableName + "has not a value");

    unique_ptr<SoundGenerator> generator = unique_ptr<SoundGenerator>(((SoundGenerator*)value->getValue())->clone());

    if( argumentValues->empty() )
        throw SemanticException("Invalid call of method setPanning without arguments.");

    if( argumentValues->size() > 1 )
        throw SemanticException("Invalid call of method setPanning more than one parameter.");

    switch( argumentValues->front()->getDataTypeId() )
    {
    case DataTypesId::Sound:
    {
        Sound panningValue = ((SoundGenerator*)argumentValues->front()->getValue())->getSound();
        generator->_baseSound.setPanning(panningValue);
        break;
    }
    case DataTypesId::Numeric:
    {
        double panningValue = *(double*)argumentValues->front()->getValue();
        generator->_baseSound.setPanning(panningValue);
        break;
    }
    default:
        throw SemanticException("Invalid cast parameter data type in setPanning method. Must be a number or a sound.");
        break;
    }

    return new SoundLiteralValue(generator->clone());
}

LiteralValue* SoundDataType::constantFreq(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in constantFreq method");

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        throw SemanticException("Variable " + variableName + "has not a value");

    unique_ptr<SoundGenerator> generator = unique_ptr<SoundGenerator>(((SoundGenerator*)value->getValue())->clone());

    if( argumentValues->empty() )
    {
        generator->_baseSound.clearAbsoluteFreq();
        return new SoundLiteralValue(generator->clone());
    }

    if( argumentValues->size() > 1 )
        throw SemanticException("Invalid call of method constantFreq more than one parameter.");

    switch( argumentValues->front()->getDataTypeId() )
    {
    case DataTypesId::Numeric:
    {
        double freqValue = *(double*)argumentValues->front()->getValue();
        generator->_baseSound.setAbsoluteFreq(freqValue);
        break;
    }
    default:
        throw SemanticException("Invalid cast parameter data type in setPanning method. Must be a number or a sound.");
        break;
    }

    return new SoundLiteralValue(generator->clone());
}

LiteralValue* SoundDataType::freqFactor(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in freqFactor method");

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        throw SemanticException("Variable " + variableName + "has not a value");

    if( argumentValues->empty() )
        throw SemanticException("Invalid call of method freqFactor without arguments.");

    if( argumentValues->size() > 1 )
        throw SemanticException("Invalid call of method freqFactor more than one parameter.");

    unique_ptr<SoundGenerator> generator = unique_ptr<SoundGenerator>(((SoundGenerator*)value->getValue())->clone());

    switch( argumentValues->front()->getDataTypeId() )
    {
    case DataTypesId::Numeric:
    {
        double factorValue = *(double*)argumentValues->front()->getValue();
        generator->_baseSound.setFreqFactor(factorValue);
        break;
    }
    default:
        throw SemanticException("Invalid cast parameter data type in freqFactor method. Must be a number or a sound.");
        break;
    }

    return new SoundLiteralValue(generator->clone());
}

LiteralValue* SoundDataType::freqModulation(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in freqModulation method");

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        throw SemanticException("Variable " + variableName + "has not a value");

    if( argumentValues->empty() )
        throw SemanticException("Invalid call of method freqOffset without arguments.");

    if( argumentValues->size() > 1 )
        throw SemanticException("Invalid call of method freqOffset more than one parameter.");

    unique_ptr<SoundGenerator> generator = unique_ptr<SoundGenerator>(((SoundGenerator*)value->getValue())->clone());

    switch( argumentValues->front()->getDataTypeId() )
    {
    case DataTypesId::Sound:
    {
        Sound offsetValue = ((SoundGenerator*)argumentValues->front()->getValue())->getSound();
        generator->_baseSound.setFreqModulation(offsetValue);
        break;
    }
    default:
        throw SemanticException("Invalid cast parameter data type in freqOffset method. Must be a number or a sound.");
        break;
    }

    return new SoundLiteralValue(generator->clone());
}

LiteralValue* SoundDataType::ampFactor(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in ampFactor method");

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        throw SemanticException("Variable " + variableName + "has not a value");

    if( argumentValues->empty() )
        throw SemanticException("Invalid call of method ampFactor without arguments.");

    if( argumentValues->size() > 1 )
        throw SemanticException("Invalid call of method ampFactor more than one parameter.");

    unique_ptr<SoundGenerator> generator = unique_ptr<SoundGenerator>(((SoundGenerator*)value->getValue())->clone());

    switch( argumentValues->front()->getDataTypeId() )
    {
    case DataTypesId::Numeric:
    {
        double factorValue = *(double*)argumentValues->front()->getValue();
        generator->_baseSound.setAmplitudeFactor(factorValue);
        break;
    }
    default:
        throw SemanticException("Invalid cast parameter data type in ampFactor method. Must be a number.");
        break;
    }

    return new SoundLiteralValue(generator->clone());
}

LiteralValue* SoundDataType::ampModulation(
    string variableName,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid argument in ampModulation method");

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        throw SemanticException("Variable " + variableName + "has not a value");

    if( argumentValues->empty() )
        throw SemanticException("Invalid call of method ampModulation without arguments.");

    if( argumentValues->size() > 1 )
        throw SemanticException("Invalid call of method ampModulation more than one parameter.");

    unique_ptr<SoundGenerator> generator = unique_ptr<SoundGenerator>(((SoundGenerator*)value->getValue())->clone());

    switch( argumentValues->front()->getDataTypeId() )
    {
    case DataTypesId::Sound:
    {
        Sound modulation = ((SoundGenerator*)argumentValues->front()->getValue())->getSound();
        generator->_baseSound.setAmplitudeModulation(modulation);
        break;
    }
    default:
        throw SemanticException("Invalid cast parameter data type in ampModulation method. Must be a sound.");
        break;
    }

    return new SoundLiteralValue(generator->clone());
}
