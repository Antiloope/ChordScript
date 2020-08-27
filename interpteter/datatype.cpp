#include "datatype.h"
#include "languageConstants.h"
#include "context.h"

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
    case DataTypesId::Function: return sFunction;
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
    if ( !dataType.compare(sFunction) ) return DataTypesId::Function;
    if ( !dataType.compare(sSound) ) return DataTypesId::Sound;
    return DataTypesId::Null;
}

#include "value.h"

DataType::DataType() {}

DataType::~DataType() {}

bool DataType::executeMethod(string methodName,LiteralValue* value,LiteralValue* arguments) {
    if( _methods.find(methodName) == _methods.end() ) return false;
    auto method = _methods.find(methodName)->second;

    LiteralValue* returnValue = method(value,arguments);
    if( !returnValue ) return false;

    Context::getInstance()->setReturnValue(returnValue);
    return true;
}

SampleDataType::SampleDataType() {}

SampleDataType::~SampleDataType() {}

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
    _methods.insert(pair<string,methodFunction_t>("play",&SoundDataType::play));
    _methods.insert(pair<string,methodFunction_t>("setPanning",&SoundDataType::setPanning));
    _methods.insert(pair<string,methodFunction_t>("constantFreq",&SoundDataType::constantFreq));
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

LiteralValue* SoundDataType::play(LiteralValue* value, LiteralValue* args) {
    if( args->getDataTypeId() != DataTypesId::Argument ) return nullptr;

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

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
        list<LiteralValue*>* arrayValues = (list<LiteralValue*>*)((ArrayLiteralValue*)(*it))->getValue();
        list<double> freqList;

        for(auto arrayIt = arrayValues->begin(); arrayIt != arrayValues->end(); arrayIt++)
        {
            if( (*arrayIt)->getDataTypeId() != DataTypesId::Numeric ) return nullptr;
            freqList.push_back(*(double*)(*arrayIt)->getValue());
        }
        it++;
        if( it == argumentValues->end() )
        {
            generator->play(freqList,0,startTick);
            return new NullLiteralValue();
        }

        if( (*it)->getDataTypeId() != DataTypesId::Numeric )
        {
            return nullptr;
        }

        double duration = *(double*)(*it)->getValue();
        it++;
        if( it != argumentValues->end() )
        {
            return nullptr;
        }

        generator->play(freqList,duration,startTick);
        break;
    }
    case DataTypesId::Argument:
    {
        for( ; it != argumentValues->end(); it++ )
        {
            list<LiteralValue*>* argumentList = (list<LiteralValue*>*)((ArgumentLiteralValue*)(*it))->getValue();
            auto argIt = argumentList->begin();

            if( (*argIt)->getDataTypeId() != DataTypesId::Array )
            {
                generator->stop();
                return nullptr;
            }

            list<LiteralValue*>* arrayValues = (list<LiteralValue*>*)((ArrayLiteralValue*)(*argIt))->getValue();
            list<double> freqList;

            for(auto arrayIt = arrayValues->begin(); arrayIt != arrayValues->end(); arrayIt++)
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
            startTick += TimeHandler::getInstance()->msToTicks(duration*1000);
        }
        break;
    }
    default:
        return nullptr;
        break;
    }

    return new NullLiteralValue();
}

LiteralValue* SoundDataType::setPanning(LiteralValue* value, LiteralValue* args) {
    if( args->getDataTypeId() != DataTypesId::Argument ) return nullptr;

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    SoundGenerator* generator = (SoundGenerator*)value->getValue();

    if( argumentValues->empty() )
    {
        throw SemanticException("Invalid call of method setPanning without arguments.");
    }

    if( argumentValues->size() > 1 )
    {
        throw SemanticException("Invalid call of method setPanning more than one parameter.");
    }

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

    return new NullLiteralValue();
}

LiteralValue* SoundDataType::constantFreq(LiteralValue* value, LiteralValue* args) {
    if( args->getDataTypeId() != DataTypesId::Argument ) return nullptr;

    auto argumentValues = (list<LiteralValue*>*)args->getValue();

    SoundGenerator* generator = ((SoundGenerator*)value->getValue())->clone();

    SoundLiteralValue* ret = new SoundLiteralValue(generator);

    if( argumentValues->empty() )
    {
        throw SemanticException("Invalid call of method constantFreq without arguments.");
    }

    if( argumentValues->size() > 1 )
    {
        throw SemanticException("Invalid call of method constantFreq more than one parameter.");
    }

    switch( argumentValues->front()->getDataTypeId() )
    {
    case DataTypesId::Sound:
    {
        Sound panningValue = ((SoundGenerator*)argumentValues->front()->getValue())->getSound();
        generator->_baseSound.setAbsoluteFreq(panningValue);
        break;
    }
    case DataTypesId::Numeric:
    {
        double panningValue = *(double*)argumentValues->front()->getValue();
        generator->_baseSound.setAbsoluteFreq(panningValue);
        break;
    }
    default:
        throw SemanticException("Invalid cast parameter data type in setPanning method. Must be a number or a sound.");
        break;
    }

    return ret;
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
        {
            tmp = true;
        }
        else
        {
            tmp = false;
        }
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

FunctionDataType::FunctionDataType() {}

FunctionDataType::~FunctionDataType() {}

LiteralValue* FunctionDataType::cast(LiteralValue* value) const {
    switch ( value->getDataTypeId() )
    {
    case DataTypesId::Function:
        return value;
        break;
    default:
        return nullptr;
        break;
    }
}
