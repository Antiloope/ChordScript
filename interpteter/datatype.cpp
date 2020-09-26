#include "datatype.h"
#include "languageConstants.h"
#include "context.h"
#include <memory>

using namespace CS;
using namespace CS::constants;

string DataType::getDataTypeString(DataTypesId dataType) {
    switch (dataType) {
    case DataTypesId::Null: return sNull;
    case DataTypesId::Numeric: return sNumber;
    case DataTypesId::Argument: return sArgument;
    case DataTypesId::Sample: return sSample;
    case DataTypesId::Operator: return "operator";
    case DataTypesId::Array: return "array";
    case DataTypesId::Group: return sGroup;
    case DataTypesId::Boolean: return sBoolean;
    case DataTypesId::String: return sString;
    case DataTypesId::Sound: return sSound;
    default: return "unknown";
    }
}

DataTypesId DataType::getDataTypeId(string dataType) {
    if ( !dataType.compare(sNumber) ) return DataTypesId::Numeric;
    if ( !dataType.compare(sArgument) ) return DataTypesId::Argument;
    if ( !dataType.compare(sSample) ) return DataTypesId::Sample;
    if ( !dataType.compare("operator") ) return DataTypesId::Operator;
    if ( !dataType.compare("array") ) return DataTypesId::Array;
    if ( !dataType.compare(sGroup) ) return DataTypesId::Group;
    if ( !dataType.compare(sBoolean) ) return DataTypesId::Boolean;
    if ( !dataType.compare(sString) ) return DataTypesId::String;
    if ( !dataType.compare(sSound) ) return DataTypesId::Sound;
    return DataTypesId::Null;
}

#include "value.h"

DataType::DataType() {}

DataType::~DataType() {}

bool DataType::executeMethod(
    string methodName,
    const LiteralValue* value,
    const LiteralValue* arguments
    ) {

    if( _methods.find(methodName) == _methods.end() )
        return false;

    auto method = _methods.find(methodName)->second;

    LiteralValue* returnValue = method(value,arguments);
    if( !returnValue )
        return false;

    Context::getInstance()->setReturnValue(returnValue);

    delete returnValue;

    return true;
}

SampleDataType::SampleDataType() {
    _methods.insert(
        pair<string,methodFunction_t>(
            "play",
            &SampleDataType::play)
        );
    _methods.insert(
        pair<string,methodFunction_t>(
            "stop",
            &SampleDataType::stop)
        );
    _methods.insert(
        pair<string,methodFunction_t>(
            "setPanning",
            &SampleDataType::setPanning)
        );
}

SampleDataType::~SampleDataType() {}

LiteralValue* SampleDataType::stop(
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

    player->stop();

    return new NullLiteralValue();
}

LiteralValue* SampleDataType::play(
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
        player->play(1,startTick);
        return new NullLiteralValue();
    }

    auto it = argumentValues->begin();

    switch( (*it)->getDataTypeId() )
    {
    case DataTypesId::Numeric:
    {
        double timeFactor = *(double*)((NumericLiteralValue*)(*it))->getValue();

        it++;
        if( it == argumentValues->end() )
        {
            player->play(timeFactor,startTick);
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
                player->stop();
                return nullptr;
            }

            double timeFactor = *(double*)(*argIt)->getValue();

            argIt++;
            if( argIt == argumentList->end() )
            {
                player->play(timeFactor, startTick);
                startTick += TimeHandler::getInstance()->segToTicks(duration);
                continue;
            }

            if( (*argIt)->getDataTypeId() != DataTypesId::Numeric )
            {
                player->stop();
                return nullptr;
            }

            double preFactor = *(double*)(*argIt)->getValue();
            argIt++;
            if( argIt != argumentList->end() )
            {
                player->stop();
                return nullptr;
            }
            if( preFactor == 0 )
            {
                startTick += TimeHandler::getInstance()->segToTicks(timeFactor);
                continue;
            }
            player->play(timeFactor,startTick);
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

SoundDataType::SoundDataType() {
    _methods.insert(
        pair<string,methodFunction_t>(
            "play",
            &SoundDataType::play)
        );
    _methods.insert(
        pair<string,methodFunction_t>(
            "stop",
            &SoundDataType::stop)
        );
    _methods.insert(
        pair<string,methodFunction_t>(
            "setPanning",
            &SoundDataType::setPanning)
        );
    _methods.insert(
        pair<string,methodFunction_t>(
            "constantFreq",
            &SoundDataType::constantFreq)
        );
    _methods.insert(
        pair<string,methodFunction_t>(
            "freqFactor",
            &SoundDataType::freqFactor)
        );
    _methods.insert(
        pair<string,methodFunction_t>(
            "freqModulation",
            &SoundDataType::freqModulation)
        );
    _methods.insert(
        pair<string,methodFunction_t>(
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

    generator->stop();

    return new NullLiteralValue();
}

LiteralValue* SoundDataType::play(
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
        generator->play({},0,startTick);
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
            generator->play(freqList,0,startTick);
            return new NullLiteralValue();
        }

        if( (*it)->getDataTypeId() != DataTypesId::Numeric )
            return nullptr;

        double duration = *(double*)(*it)->getValue();
        it++;

        if( it != argumentValues->end() )
            return nullptr;

        generator->play(freqList,duration,startTick);
        break;
    }
    case DataTypesId::Argument:
    {
        for( ; it != argumentValues->end(); it++ )
        {
            const list<LiteralValue*>* argumentList = (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();
            auto argIt = argumentList->begin();

            if( (*argIt)->getDataTypeId() != DataTypesId::Array )
            {
                generator->stop();
                return nullptr;
            }

            const list<LiteralValue*>* arrayValues = (list<LiteralValue*>*)((ArrayLiteralValue*)(*argIt))->getValue();
            list<double> freqList;

            for( auto arrayIt = arrayValues->begin(); arrayIt != arrayValues->end(); arrayIt++ )
            {
                if( (*arrayIt)->getDataTypeId() != DataTypesId::Numeric )
                {
                    generator->stop();
                    return nullptr;
                }
                freqList.push_back(*(double*)(*arrayIt)->getValue());
            }
            argIt++;
            if( argIt == argumentList->end() )
            {
                generator->stop();
                return nullptr;
            }

            if( (*argIt)->getDataTypeId() != DataTypesId::Numeric )
            {
                generator->stop();
                return nullptr;
            }

            double duration = *(double*)(*argIt)->getValue();
            argIt++;

            if( argIt != argumentList->end() )
            {
                generator->stop();
                return nullptr;
            }

            generator->play(freqList,duration,startTick);
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

NumericDataType::NumericDataType() {}

NumericDataType::~NumericDataType() {}

LiteralValue* NumericDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Numeric:
        return value;
        break;
    case DataTypesId::Boolean:
    {
        int returnValue = *(bool*)value->getValue();
        delete value;
        return new NumericLiteralValue(returnValue);
    }
        break;
    default:
        return nullptr;
        break;
    }
}

ArgumentDataType::ArgumentDataType() {}

ArgumentDataType::~ArgumentDataType() {}

LiteralValue* ArgumentDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Argument:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}

GroupDataType::GroupDataType() {}

GroupDataType::~GroupDataType() {}

LiteralValue* GroupDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Group:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}

BooleanDataType::BooleanDataType() {}

BooleanDataType::~BooleanDataType() {}

LiteralValue* BooleanDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Boolean:
        return value;
        break;
    case DataTypesId::Numeric:
    {
        bool returnValue = *(double*)value->getValue();
        delete value;
        return new BooleanLiteralValue(returnValue);
    }
    case DataTypesId::Null:
    {
        delete value;
        return new BooleanLiteralValue(false);
    }
    case DataTypesId::String:
    {
        string returnValue = *(string*)value->getValue();
        bool tmp;
        if( returnValue.length() )
            tmp = true;
        else
            tmp = false;
        delete value;
        return new BooleanLiteralValue(tmp);
    }
    default:
        return nullptr;
        break;
    }
}

StringDataType::StringDataType() {}

StringDataType::~StringDataType() {}

LiteralValue* StringDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::String:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}

NullDataType::NullDataType() {}

NullDataType::~NullDataType() {}

LiteralValue* NullDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Null:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}
