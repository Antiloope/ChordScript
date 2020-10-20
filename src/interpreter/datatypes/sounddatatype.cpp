#include "sounddatatype.h"
#include "interpreter/context.h"
#include "interpreter/values/literalvalue.h"
#include <memory>
#include "interpreter/players/soundgenerator.h"
#include "utils/Exceptions/exception.h"

using namespace CS;
using namespace CS::Constants;
using namespace std;

SoundDataType::SoundDataType() {
    _methods.insert(
        pair<string,method_function_t>(
            "play",
            &SoundDataType::play)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "loop",
            &SoundDataType::loop)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "stop",
            &SoundDataType::stop)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "setPanning",
            &SoundDataType::setPanning)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "constantFreq",
            &SoundDataType::constantFreq)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "freqFactor",
            &SoundDataType::freqFactor)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "freqModulation",
            &SoundDataType::freqModulation)
        );
    _methods.insert(
        pair<string,method_function_t>(
            "ampFactor",
            &SoundDataType::ampFactor)
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
        return nullptr;

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !argumentValues->empty() )
        return nullptr;

    if( !value )
        return nullptr;

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
        return nullptr;

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        return nullptr;

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
                return nullptr;

            freqList.push_back(*(double*)(*arrayIt)->getValue());
        }
        it++;
        if( it == argumentValues->end() )
        {
            generator->play(freqList,0,startTick,variableName);
            return new NullLiteralValue();
        }

        if( (*it)->getDataTypeId() != DataTypesId::Numeric )
            return nullptr;

        double duration = *(double*)(*it)->getValue();
        it++;

        if( it != argumentValues->end() )
            return nullptr;

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
                return nullptr;

            const list<LiteralValue*>* arrayValues = (list<LiteralValue*>*)((ArrayLiteralValue*)(*argIt))->getValue();
            list<double> freqList;

            for( auto arrayIt = arrayValues->begin(); arrayIt != arrayValues->end(); arrayIt++ )
            {
                if( (*arrayIt)->getDataTypeId() != DataTypesId::Numeric )
                    return nullptr;

                freqList.push_back(*(double*)(*arrayIt)->getValue());
            }
            argIt++;
            if( argIt == argumentList->end() )
            {
                generator->play(freqList,0,startTick,variableName);
                return new NullLiteralValue();
            }

            if( (*argIt)->getDataTypeId() != DataTypesId::Numeric )
                return nullptr;

            double duration = *(double*)(*argIt)->getValue();
            argIt++;

            if( argIt != argumentList->end() )
                return nullptr;


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
        return nullptr;

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        return nullptr;

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
                return nullptr;

            freqList.push_back(*(double*)(*arrayIt)->getValue());
        }
        it++;
        if( it == argumentValues->end() )
        {
            generator->play(freqList,0,startTick,variableName);
            return new NullLiteralValue();
        }

        if( (*it)->getDataTypeId() != DataTypesId::Numeric )
            return nullptr;

        double duration = *(double*)(*it)->getValue();
        it++;

        if( it != argumentValues->end() )
            return nullptr;

        generator->loop(freqList,duration,duration,startTick,variableName);
        break;
    }
    case DataTypesId::Argument:
    {
        double totalLoopDuration = 0;
        for( ; it != argumentValues->end(); it++ )
        {
            const list<LiteralValue*>* argumentList = (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();
            auto argIt = argumentList->begin();

            if( (*argIt)->getDataTypeId() != DataTypesId::Array )
                return nullptr;

            const list<LiteralValue*>* arrayValues = (list<LiteralValue*>*)((ArrayLiteralValue*)(*argIt))->getValue();
            list<double> freqList;

            for( auto arrayIt = arrayValues->begin(); arrayIt != arrayValues->end(); arrayIt++ )
            {
                if( (*arrayIt)->getDataTypeId() != DataTypesId::Numeric )
                    return nullptr;
                freqList.push_back(*(double*)(*arrayIt)->getValue());
            }
            argIt++;
            if( argIt == argumentList->end() )
                return nullptr;

            if( (*argIt)->getDataTypeId() != DataTypesId::Numeric )
                return nullptr;

            totalLoopDuration += *(double*)(*argIt)->getValue();
            argIt++;

            if( argIt != argumentList->end() )
                return nullptr;
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
    string,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        return nullptr;

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        return nullptr;

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
    string,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        return nullptr;

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        return nullptr;

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
    string,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        return nullptr;

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        return nullptr;

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
    string,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        return nullptr;

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        return nullptr;

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
    string,
    const LiteralValue* value,
    const LiteralValue* args
    ) {

    if( args->getDataTypeId() != DataTypesId::Argument )
        return nullptr;

    const auto argumentValues = (list<LiteralValue*>*)args->getValue();

    if( !value )
        return nullptr;

    if( argumentValues->empty() )
        throw SemanticException("Invalid call of method ampFactor without arguments.");

    if( argumentValues->size() > 1 )
        throw SemanticException("Invalid call of method ampFactor more than one parameter.");

    unique_ptr<SoundGenerator> generator = unique_ptr<SoundGenerator>(((SoundGenerator*)value->getValue())->clone());

    switch( argumentValues->front()->getDataTypeId() )
    {
    case DataTypesId::Sound:
    {
        Sound factorValue = ((SoundGenerator*)argumentValues->front()->getValue())->getSound();
        generator->_baseSound.setAmplitudeFactor(factorValue);
        break;
    }
    case DataTypesId::Numeric:
    {
        double factorValue = *(double*)argumentValues->front()->getValue();
        generator->_baseSound.setAmplitudeFactor(factorValue);
        break;
    }
    default:
        throw SemanticException("Invalid cast parameter data type in ampFactor method. Must be a number or a sound.");
        break;
    }

    return new SoundLiteralValue(generator->clone());
}
