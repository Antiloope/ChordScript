#include "context.h"
#include "functiondefinition.h"
#include "languagedefinitions.h"
#include <memory>
#include "values/literalvalue.h"
#include "players/soundgenerator.h"

using namespace std;
using namespace CS;

const scope_index MAX_CONTEXT_COUNT = 1000;

Context* Context::_instance = nullptr;

Scope::Scope() {}

Scope::Scope(const Scope& scope) {
    for( auto var : scope._scope ) {
        get<1>(get<1>(var)) = get<1>(get<1>(var))->clone();
        _scope.insert(var);
    }
}

Scope::~Scope() {
    for( auto it : _scope )
        if ( get<1>(it.second) )
            delete get<1>(it.second);
}

bool Scope::exist(string name) {
    if( _scope.find(name) != _scope.end() )
        return true;
    return false;
}

DataTypesId Scope::getVariableDataType(string name) {
    if( !get<1>(_scope.find(name)->second) )
        return DataTypesId::Null;
    return get<1>(_scope.find(name)->second)->getDataTypeId();
}

DataTypesId Scope::getDefinedDataType(string name) {
    return get<0>(_scope.find(name)->second);
}

void Scope::newVariable(string name, DataTypesId dataType) {
    if( _scope.find(name) != _scope.end() )
    {
        if( get<1>(_scope.find(name)->second) )
            delete get<1>(_scope.find(name)->second);
        _scope.erase(name);
    }
    _scope.insert(
        { name,tuple<DataTypesId,LiteralValue*>(dataType,new NullLiteralValue()) });
}

LiteralValue* Scope::getVariableValue(string name) {
    if( !get<1>(_scope.find(name)->second) )
        return nullptr;
    return get<1>(_scope.find(name)->second)->clone();
}

bool Scope::setVariableValue(string name,LiteralValue* value) {
    if( _scope.find(name) == _scope.end() )
        return false;
    if( get<1>(_scope.find(name)->second) )
        delete get<1>(_scope.find(name)->second);

    get<1>(_scope.find(name)->second) = value;
    return true;
}

const variables_map Scope::getAllPlayables() {
    variables_map ret;

    for( auto&& scope : _scope )
    {
        if(
            get<0>(scope.second) == DataTypesId::Sample ||
            get<0>(scope.second) == DataTypesId::Sound
            )
            ret.insert(scope);
    }

    return ret;
}

Context::Context() {
    _dataTypes = {
        {DataTypesId::Sample,      new SampleDataType()},
        {DataTypesId::Sound,       new SoundDataType()},
        {DataTypesId::Numeric,     new NumberDataType()},
        {DataTypesId::Argument,    new ArgumentDataType()},
        {DataTypesId::Group,       new GroupDataType()},
        {DataTypesId::Boolean,     new BooleanDataType()},
        {DataTypesId::String,      new StringDataType()},
    };

    _scopes.insert( pair<size_t,Scope>(GLOBAL_SCOPE,Scope()) );

    _functions = {};

    _audioFiles = {};

    _scopeStack.push(GLOBAL_SCOPE);

    _returnValue = nullptr;

    for(scope_index i = MAX_CONTEXT_COUNT ; i != 0 ; i--)
    {
        _freeScopesIndexes.push(i);
    }
}

void Context::load() {
    this->newVariable(Names::sinSound,DataTypesId::Sound);
    this->newVariable(Names::squareSound,DataTypesId::Sound);

    LiteralValue* value = new SoundLiteralValue(new SoundGenerator(&Functions::sin));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::sinSound,value);

    value = new SoundLiteralValue(new SoundGenerator(&Functions::square));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::squareSound,value);

    this->newVariable(Names::c0,DataTypesId::Numeric);
    this->newVariable(Names::cs0,DataTypesId::Numeric);
    this->newVariable(Names::db0,DataTypesId::Numeric);
    this->newVariable(Names::d0,DataTypesId::Numeric);
    this->newVariable(Names::ds0,DataTypesId::Numeric);
    this->newVariable(Names::eb0,DataTypesId::Numeric);
    this->newVariable(Names::e0,DataTypesId::Numeric);
    this->newVariable(Names::f0,DataTypesId::Numeric);
    this->newVariable(Names::fs0,DataTypesId::Numeric);
    this->newVariable(Names::gb0,DataTypesId::Numeric);
    this->newVariable(Names::g0,DataTypesId::Numeric);
    this->newVariable(Names::gs0,DataTypesId::Numeric);
    this->newVariable(Names::ab0,DataTypesId::Numeric);
    this->newVariable(Names::a0,DataTypesId::Numeric);
    this->newVariable(Names::as0,DataTypesId::Numeric);
    this->newVariable(Names::bb0,DataTypesId::Numeric);
    this->newVariable(Names::b0,DataTypesId::Numeric);
    this->newVariable(Names::c1,DataTypesId::Numeric);
    this->newVariable(Names::cs1,DataTypesId::Numeric);
    this->newVariable(Names::db1,DataTypesId::Numeric);
    this->newVariable(Names::d1,DataTypesId::Numeric);
    this->newVariable(Names::ds1,DataTypesId::Numeric);
    this->newVariable(Names::eb1,DataTypesId::Numeric);
    this->newVariable(Names::e1,DataTypesId::Numeric);
    this->newVariable(Names::f1,DataTypesId::Numeric);
    this->newVariable(Names::fs1,DataTypesId::Numeric);
    this->newVariable(Names::gb1,DataTypesId::Numeric);
    this->newVariable(Names::g1,DataTypesId::Numeric);
    this->newVariable(Names::gs1,DataTypesId::Numeric);
    this->newVariable(Names::ab1,DataTypesId::Numeric);
    this->newVariable(Names::a1,DataTypesId::Numeric);
    this->newVariable(Names::as1,DataTypesId::Numeric);
    this->newVariable(Names::bb1,DataTypesId::Numeric);
    this->newVariable(Names::b1,DataTypesId::Numeric);
    this->newVariable(Names::c2,DataTypesId::Numeric);
    this->newVariable(Names::cs2,DataTypesId::Numeric);
    this->newVariable(Names::db2,DataTypesId::Numeric);
    this->newVariable(Names::d2,DataTypesId::Numeric);
    this->newVariable(Names::ds2,DataTypesId::Numeric);
    this->newVariable(Names::eb2,DataTypesId::Numeric);
    this->newVariable(Names::e2,DataTypesId::Numeric);
    this->newVariable(Names::f2,DataTypesId::Numeric);
    this->newVariable(Names::fs2,DataTypesId::Numeric);
    this->newVariable(Names::gb2,DataTypesId::Numeric);
    this->newVariable(Names::g2,DataTypesId::Numeric);
    this->newVariable(Names::gs2,DataTypesId::Numeric);
    this->newVariable(Names::ab2,DataTypesId::Numeric);
    this->newVariable(Names::a2,DataTypesId::Numeric);
    this->newVariable(Names::as2,DataTypesId::Numeric);
    this->newVariable(Names::bb2,DataTypesId::Numeric);
    this->newVariable(Names::b2,DataTypesId::Numeric);
    this->newVariable(Names::c3,DataTypesId::Numeric);
    this->newVariable(Names::cs3,DataTypesId::Numeric);
    this->newVariable(Names::db3,DataTypesId::Numeric);
    this->newVariable(Names::d3,DataTypesId::Numeric);
    this->newVariable(Names::ds3,DataTypesId::Numeric);
    this->newVariable(Names::eb3,DataTypesId::Numeric);
    this->newVariable(Names::e3,DataTypesId::Numeric);
    this->newVariable(Names::f3,DataTypesId::Numeric);
    this->newVariable(Names::fs3,DataTypesId::Numeric);
    this->newVariable(Names::gb3,DataTypesId::Numeric);
    this->newVariable(Names::g3,DataTypesId::Numeric);
    this->newVariable(Names::gs3,DataTypesId::Numeric);
    this->newVariable(Names::ab3,DataTypesId::Numeric);
    this->newVariable(Names::a3,DataTypesId::Numeric);
    this->newVariable(Names::as3,DataTypesId::Numeric);
    this->newVariable(Names::bb3,DataTypesId::Numeric);
    this->newVariable(Names::b3,DataTypesId::Numeric);
    this->newVariable(Names::c4,DataTypesId::Numeric);
    this->newVariable(Names::cs4,DataTypesId::Numeric);
    this->newVariable(Names::db4,DataTypesId::Numeric);
    this->newVariable(Names::d4,DataTypesId::Numeric);
    this->newVariable(Names::ds4,DataTypesId::Numeric);
    this->newVariable(Names::eb4,DataTypesId::Numeric);
    this->newVariable(Names::e4,DataTypesId::Numeric);
    this->newVariable(Names::f4,DataTypesId::Numeric);
    this->newVariable(Names::fs4,DataTypesId::Numeric);
    this->newVariable(Names::gb4,DataTypesId::Numeric);
    this->newVariable(Names::g4,DataTypesId::Numeric);
    this->newVariable(Names::gs4,DataTypesId::Numeric);
    this->newVariable(Names::ab4,DataTypesId::Numeric);
    this->newVariable(Names::a4,DataTypesId::Numeric);
    this->newVariable(Names::as4,DataTypesId::Numeric);
    this->newVariable(Names::bb4,DataTypesId::Numeric);
    this->newVariable(Names::b4,DataTypesId::Numeric);
    this->newVariable(Names::c5,DataTypesId::Numeric);
    this->newVariable(Names::cs5,DataTypesId::Numeric);
    this->newVariable(Names::db5,DataTypesId::Numeric);
    this->newVariable(Names::d5,DataTypesId::Numeric);
    this->newVariable(Names::ds5,DataTypesId::Numeric);
    this->newVariable(Names::eb5,DataTypesId::Numeric);
    this->newVariable(Names::e5,DataTypesId::Numeric);
    this->newVariable(Names::f5,DataTypesId::Numeric);
    this->newVariable(Names::fs5,DataTypesId::Numeric);
    this->newVariable(Names::gb5,DataTypesId::Numeric);
    this->newVariable(Names::g5,DataTypesId::Numeric);
    this->newVariable(Names::gs5,DataTypesId::Numeric);
    this->newVariable(Names::ab5,DataTypesId::Numeric);
    this->newVariable(Names::a5,DataTypesId::Numeric);
    this->newVariable(Names::as5,DataTypesId::Numeric);
    this->newVariable(Names::bb5,DataTypesId::Numeric);
    this->newVariable(Names::b5,DataTypesId::Numeric);
    this->newVariable(Names::c6,DataTypesId::Numeric);
    this->newVariable(Names::cs6,DataTypesId::Numeric);
    this->newVariable(Names::db6,DataTypesId::Numeric);
    this->newVariable(Names::d6,DataTypesId::Numeric);
    this->newVariable(Names::ds6,DataTypesId::Numeric);
    this->newVariable(Names::eb6,DataTypesId::Numeric);
    this->newVariable(Names::e6,DataTypesId::Numeric);
    this->newVariable(Names::f6,DataTypesId::Numeric);
    this->newVariable(Names::fs6,DataTypesId::Numeric);
    this->newVariable(Names::gb6,DataTypesId::Numeric);
    this->newVariable(Names::g6,DataTypesId::Numeric);
    this->newVariable(Names::gs6,DataTypesId::Numeric);
    this->newVariable(Names::ab6,DataTypesId::Numeric);
    this->newVariable(Names::a6,DataTypesId::Numeric);
    this->newVariable(Names::as6,DataTypesId::Numeric);
    this->newVariable(Names::bb6,DataTypesId::Numeric);
    this->newVariable(Names::b6,DataTypesId::Numeric);
    this->newVariable(Names::c7,DataTypesId::Numeric);
    this->newVariable(Names::cs7,DataTypesId::Numeric);
    this->newVariable(Names::db7,DataTypesId::Numeric);
    this->newVariable(Names::d7,DataTypesId::Numeric);
    this->newVariable(Names::ds7,DataTypesId::Numeric);
    this->newVariable(Names::eb7,DataTypesId::Numeric);
    this->newVariable(Names::e7,DataTypesId::Numeric);
    this->newVariable(Names::f7,DataTypesId::Numeric);
    this->newVariable(Names::fs7,DataTypesId::Numeric);
    this->newVariable(Names::gb7,DataTypesId::Numeric);
    this->newVariable(Names::g7,DataTypesId::Numeric);
    this->newVariable(Names::gs7,DataTypesId::Numeric);
    this->newVariable(Names::ab7,DataTypesId::Numeric);
    this->newVariable(Names::a7,DataTypesId::Numeric);
    this->newVariable(Names::as7,DataTypesId::Numeric);
    this->newVariable(Names::bb7,DataTypesId::Numeric);
    this->newVariable(Names::b7,DataTypesId::Numeric);
    this->newVariable(Names::c8,DataTypesId::Numeric);
    this->newVariable(Names::cs8,DataTypesId::Numeric);
    this->newVariable(Names::db8,DataTypesId::Numeric);
    this->newVariable(Names::d8,DataTypesId::Numeric);
    this->newVariable(Names::ds8,DataTypesId::Numeric);
    this->newVariable(Names::eb8,DataTypesId::Numeric);
    this->newVariable(Names::e8,DataTypesId::Numeric);
    this->newVariable(Names::f8,DataTypesId::Numeric);
    this->newVariable(Names::fs8,DataTypesId::Numeric);
    this->newVariable(Names::gb8,DataTypesId::Numeric);
    this->newVariable(Names::g8,DataTypesId::Numeric);
    this->newVariable(Names::gs8,DataTypesId::Numeric);
    this->newVariable(Names::ab8,DataTypesId::Numeric);
    this->newVariable(Names::a8,DataTypesId::Numeric);
    this->newVariable(Names::as8,DataTypesId::Numeric);
    this->newVariable(Names::bb8,DataTypesId::Numeric);
    this->newVariable(Names::b8,DataTypesId::Numeric);

    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::c0,new NumberLiteralValue(16.351));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::cs0,new NumberLiteralValue(17.324));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::db0,new NumberLiteralValue(17.324));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::d0,new NumberLiteralValue(18.354));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ds0,new NumberLiteralValue(19.445));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::eb0,new NumberLiteralValue(19.445));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::e0,new NumberLiteralValue(20.601));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::f0,new NumberLiteralValue(21.827));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::fs0,new NumberLiteralValue(23.124));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gb0,new NumberLiteralValue(23.124));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::g0,new NumberLiteralValue(24.499));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gs0,new NumberLiteralValue(25.956));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ab0,new NumberLiteralValue(25.956));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::a0,new NumberLiteralValue(27.5));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::as0,new NumberLiteralValue(29.135));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::bb0,new NumberLiteralValue(29.135));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::b0,new NumberLiteralValue(30.868));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::c1,new NumberLiteralValue(32.703));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::cs1,new NumberLiteralValue(34.648));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::db1,new NumberLiteralValue(34.648));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::d1,new NumberLiteralValue(36.708));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ds1,new NumberLiteralValue(38.891));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::eb1,new NumberLiteralValue(38.891));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::e1,new NumberLiteralValue(41.203));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::f1,new NumberLiteralValue(43.654));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::fs1,new NumberLiteralValue(46.249));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gb1,new NumberLiteralValue(46.249));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::g1,new NumberLiteralValue(48.999));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gs1,new NumberLiteralValue(51.913));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ab1,new NumberLiteralValue(51.913));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::a1,new NumberLiteralValue(55));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::as1,new NumberLiteralValue(58.27));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::bb1,new NumberLiteralValue(58.27));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::b1,new NumberLiteralValue(61.735));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::c2,new NumberLiteralValue(65.406));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::cs2,new NumberLiteralValue(69.296));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::db2,new NumberLiteralValue(69.296));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::d2,new NumberLiteralValue(73.416));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ds2,new NumberLiteralValue(77.782));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::eb2,new NumberLiteralValue(77.782));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::e2,new NumberLiteralValue(82.407));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::f2,new NumberLiteralValue(87.307));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::fs2,new NumberLiteralValue(92.499));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gb2,new NumberLiteralValue(92.499));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::g2,new NumberLiteralValue(97.999));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gs2,new NumberLiteralValue(103.826));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ab2,new NumberLiteralValue(103.826));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::a2,new NumberLiteralValue(110));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::as2,new NumberLiteralValue(116.541));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::bb2,new NumberLiteralValue(116.541));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::b2,new NumberLiteralValue(123.471));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::c3,new NumberLiteralValue(130.813));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::cs3,new NumberLiteralValue(138.591));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::db3,new NumberLiteralValue(138.591));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::d3,new NumberLiteralValue(146.832));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ds3,new NumberLiteralValue(155.563));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::eb3,new NumberLiteralValue(155.563));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::e3,new NumberLiteralValue(164.814));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::f3,new NumberLiteralValue(174.614));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::fs3,new NumberLiteralValue(184.997));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gb3,new NumberLiteralValue(184.997));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::g3,new NumberLiteralValue(195.998));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gs3,new NumberLiteralValue(207.652));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ab3,new NumberLiteralValue(207.652));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::a3,new NumberLiteralValue(220));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::as3,new NumberLiteralValue(233.082));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::bb3,new NumberLiteralValue(233.082));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::b3,new NumberLiteralValue(246.942));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::c4,new NumberLiteralValue(261.626));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::cs4,new NumberLiteralValue(277.183));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::db4,new NumberLiteralValue(277.183));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::d4,new NumberLiteralValue(293.665));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ds4,new NumberLiteralValue(311.127));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::eb4,new NumberLiteralValue(311.127));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::e4,new NumberLiteralValue(329.628));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::f4,new NumberLiteralValue(349.228));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::fs4,new NumberLiteralValue(369.994));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gb4,new NumberLiteralValue(369.994));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::g4,new NumberLiteralValue(391.995));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gs4,new NumberLiteralValue(415.305));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ab4,new NumberLiteralValue(415.305));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::a4,new NumberLiteralValue(440));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::as4,new NumberLiteralValue(466.164));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::bb4,new NumberLiteralValue(466.164));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::b4,new NumberLiteralValue(493.883));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::c5,new NumberLiteralValue(523.251));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::cs5,new NumberLiteralValue(554.365));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::db5,new NumberLiteralValue(554.365));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::d5,new NumberLiteralValue(587.33));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ds5,new NumberLiteralValue(622.254));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::eb5,new NumberLiteralValue(622.254));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::e5,new NumberLiteralValue(659.255));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::f5,new NumberLiteralValue(698.456));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::fs5,new NumberLiteralValue(739.989));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gb5,new NumberLiteralValue(739.989));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::g5,new NumberLiteralValue(783.991));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gs5,new NumberLiteralValue(830.609));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ab5,new NumberLiteralValue(830.609));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::a5,new NumberLiteralValue(880));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::as5,new NumberLiteralValue(932.328));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::bb5,new NumberLiteralValue(932.328));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::b5,new NumberLiteralValue(987.767));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::c6,new NumberLiteralValue(1046.502));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::cs6,new NumberLiteralValue(1108.731));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::db6,new NumberLiteralValue(1108.731));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::d6,new NumberLiteralValue(1174.659));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ds6,new NumberLiteralValue(1244.508));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::eb6,new NumberLiteralValue(1244.508));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::e6,new NumberLiteralValue(1318.51));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::f6,new NumberLiteralValue(1396.913));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::fs6,new NumberLiteralValue(1479.978));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gb6,new NumberLiteralValue(1479.978));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::g6,new NumberLiteralValue(1567.982));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gs6,new NumberLiteralValue(1661.219));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ab6,new NumberLiteralValue(1661.219));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::a6,new NumberLiteralValue(1760));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::as6,new NumberLiteralValue(1864.655));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::bb6,new NumberLiteralValue(1864.655));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::b6,new NumberLiteralValue(1975.533));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::c7,new NumberLiteralValue(2093.005));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::cs7,new NumberLiteralValue(2217.461));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::db7,new NumberLiteralValue(2217.461));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::d7,new NumberLiteralValue(2349.318));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ds7,new NumberLiteralValue(2489.016));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::eb7,new NumberLiteralValue(2489.016));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::e7,new NumberLiteralValue(2637.021));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::f7,new NumberLiteralValue(2793.826));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::fs7,new NumberLiteralValue(2959.955));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gb7,new NumberLiteralValue(2959.955));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::g7,new NumberLiteralValue(3135.964));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gs7,new NumberLiteralValue(3322.438));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ab7,new NumberLiteralValue(3322.438));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::a7,new NumberLiteralValue(3520));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::as7,new NumberLiteralValue(3729.31));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::bb7,new NumberLiteralValue(3729.31));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::b7,new NumberLiteralValue(3951.066));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::c8,new NumberLiteralValue(4186.009));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::cs8,new NumberLiteralValue(4434.922));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::db8,new NumberLiteralValue(4434.922));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::d8,new NumberLiteralValue(4698.636));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ds8,new NumberLiteralValue(4978.032));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::eb8,new NumberLiteralValue(4978.032));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::e8,new NumberLiteralValue(5274.042));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::f8,new NumberLiteralValue(5587.652));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::fs8,new NumberLiteralValue(5919.91));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gb8,new NumberLiteralValue(5919.91));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::g8,new NumberLiteralValue(6271.928));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::gs8,new NumberLiteralValue(6644.876));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::ab8,new NumberLiteralValue(6644.876));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::a8,new NumberLiteralValue(7040));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::as8,new NumberLiteralValue(7458.62));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::bb8,new NumberLiteralValue(7458.62));
    _scopes.find(GLOBAL_SCOPE)->second.setVariableValue(Names::b8,new NumberLiteralValue(7902.132));

    BaseFunction* func = new BaseFunction();
    func->load({{DataTypesId::String,"fileName"}},&Functions::sample);
    _functions.insert({Names::sampleFunction,tuple<DataTypesId,BaseFunction*>(DataTypesId::Sample,func)});

    func = new BaseFunction();
    func->load({{}},&Functions::stop);
    _functions.insert({Names::stopFunction,tuple<DataTypesId,BaseFunction*>(DataTypesId::Null,func)});

    func = new BaseFunction();
    func->load({{}},&Functions::startRecording);
    _functions.insert({Names::startRecordingFunction,tuple<DataTypesId,BaseFunction*>(DataTypesId::Null,func)});

    func = new BaseFunction();
    func->load({{}},&Functions::stopRecording);
    _functions.insert({Names::stopRecordingFunction,tuple<DataTypesId,BaseFunction*>(DataTypesId::Null,func)});

    func = new BaseFunction();
    func->load({{}},&Functions::startServer);
    _functions.insert({Names::startServerFunction,tuple<DataTypesId,BaseFunction*>(DataTypesId::Null,func)});

    func = new BaseFunction();
    func->load({{}},&Functions::restartServer);
    _functions.insert({Names::restartServerFunction,tuple<DataTypesId,BaseFunction*>(DataTypesId::Null,func)});

    func = new BaseFunction();
    func->load({{}},&Functions::killServer);
    _functions.insert({Names::killServerFunction,tuple<DataTypesId,BaseFunction*>(DataTypesId::Null,func)});
}

scope_index Context::newScope() {
    scope_index ctxId = _freeScopesIndexes.top();
    _freeScopesIndexes.pop();
    _scopes.insert(
        pair<scope_index,Scope>(ctxId,Scope()));
    _scopeStack.push(ctxId);
    return ctxId;
}

void Context::newAudioFile(string fileName,AudioFile<float> audioFile) {
    _audioFiles.erase(fileName);
    _audioFiles.insert(
        pair<string,AudioFile<float>>(fileName,audioFile));
}

void Context::removeAudioFile(string fileName) {
    _audioFiles.erase(fileName);
}

const AudioFile<float>* Context::getAudioFile(string fileName) const {
    auto file = _audioFiles.find(fileName);
    if( file == _audioFiles.end() )
        return nullptr;

    return &file->second;
}

void Context::returnScope() {
    if ( _scopeStack.top() )
        _scopeStack.pop();
}

scope_index Context::switchScope(scope_index scopeIndex) {
    if( _scopes.find(scopeIndex) == _scopes.end() )
        return GLOBAL_SCOPE;

    scope_index returnScopeId = _freeScopesIndexes.top();
    _freeScopesIndexes.pop();
    _scopes.insert(
        pair<scope_index,Scope>(
            returnScopeId,
            Scope(_scopes.find(scopeIndex)->second))
        );

    _scopeStack.push(returnScopeId);
    return returnScopeId;
}

scope_index Context::getCurrentScope() const {
    return _scopeStack.top();
}

bool Context::isDataType(DataTypesId dataType) const {
    return _dataTypes.find(dataType) != _dataTypes.end();
}

bool Context::isValidName(string name) const {
    return Constants::isValidName(name);
}

DataTypesId Context::getVariableDataType(string name) {
    stack<scope_index> tmpStack;
    while( !_scopeStack.empty() )
    {
        if( !_scopes.find(_scopeStack.top())->second.exist(name) )
        {
            tmpStack.push(_scopeStack.top());
            _scopeStack.pop();
        }
        else break;
    }
    if( _scopeStack.empty() )
    {
        while( !tmpStack.empty() )
        {
            _scopeStack.push(tmpStack.top());
            tmpStack.pop();
        }
        return DataTypesId::Null;
    }
    scope_index currentScope = _scopeStack.top();
    while( !tmpStack.empty() )
    {
        _scopeStack.push(tmpStack.top());
        tmpStack.pop();
    }

    return _scopes.find(currentScope)->second.getVariableDataType(name);
}

bool Context::nameExist(string name) {
    stack<scope_index> tmpStack;
    while( !_scopeStack.empty() )
    {
        if( !_scopes.find(_scopeStack.top())->second.exist(name) )
        {
            tmpStack.push(_scopeStack.top());
            _scopeStack.pop();
        }
        else break;
    }
    if( _scopeStack.empty() )
    {
        while( !tmpStack.empty() )
        {
            _scopeStack.push(tmpStack.top());
            tmpStack.pop();
        }
        return false;
    }
    while( !tmpStack.empty() )
    {
        _scopeStack.push(tmpStack.top());
        tmpStack.pop();
    }
    return true;
}

bool Context::functionNameExist(string name) const {
    return _functions.find(name) != _functions.end();
}

void Context::newVariable(string name, DataTypesId dataType) {
    scope_index currentContext = _scopeStack.top();
    _scopes.find(currentContext)->second.newVariable(name,dataType);
}

FunctionDefinition* Context::newFunction(string name, DataTypesId dataType) {
    auto tmp = _functions.find(name);
    if(tmp != _functions.end())
    {
        delete get<1>(tmp->second);
        _functions.erase(name);
    }
    FunctionDefinition* ret = new FunctionDefinition();
    _functions.insert(
        {
            name,
            tuple<DataTypesId,FunctionDefinition*>(
                dataType,
                ret)
        });
    return ret;
}

bool Context::executeMethod(string variableName,string methodName,LiteralValue* arguments) {
    stack<scope_index> tmpStack;
    while( !_scopeStack.empty() )
    {
        if( !_scopes.find(_scopeStack.top())->second.exist(variableName) )
        {
            tmpStack.push(_scopeStack.top());
            _scopeStack.pop();
        }
        else break;
    }
    if( _scopeStack.empty() )
    {
        while( !tmpStack.empty() )
        {
            _scopeStack.push(tmpStack.top());
            tmpStack.pop();
        }
        return false;
    }
    DataTypesId dataTypeId = _scopes.find(_scopeStack.top())->second.getDefinedDataType(variableName);
    unique_ptr<LiteralValue> literalValue = unique_ptr<LiteralValue>(_scopes.find(_scopeStack.top())->second.getVariableValue(variableName));

    while( !tmpStack.empty() )
    {
        _scopeStack.push(tmpStack.top());
        tmpStack.pop();
    }

    DataType* dataType = _dataTypes.find(dataTypeId)->second;

    if( !dataType->executeMethod(methodName,variableName,literalValue.get(),arguments) )
    {
        return false;
    }

    return true;
}

LiteralValue* Context::getVariableValue(string name) {
    stack<scope_index> tmpStack;
    while( !_scopeStack.empty() )
    {
        if( !_scopes.find(_scopeStack.top())->second.exist(name) )
        {
            tmpStack.push(_scopeStack.top());
            _scopeStack.pop();
        }
        else
            break;
    }
    if( _scopeStack.empty() )
    {
        while( !tmpStack.empty() )
        {
            _scopeStack.push(tmpStack.top());
            tmpStack.pop();
        }
        return nullptr;
    }
    size_t ctx = _scopeStack.top();
    while( !tmpStack.empty() )
    {
        _scopeStack.push(tmpStack.top());
        tmpStack.pop();
    }

    return _scopes.find(ctx)->second.getVariableValue(name);
}

LiteralValue* Context::getVariableValue(string name,DataTypesId dataTypeId) {
    stack<scope_index> tmpStack;
    while( !_scopeStack.empty() )
    {
        if( !_scopes.find(_scopeStack.top())->second.exist(name) )
        {
            tmpStack.push(_scopeStack.top());
            _scopeStack.pop();
        }
        else
            break;
    }
    if( _scopeStack.empty() )
    {
        while( !tmpStack.empty() )
        {
            _scopeStack.push(tmpStack.top());
            tmpStack.pop();
        }
        return nullptr;
    }
    size_t ctxIndex = _scopeStack.top();
    while( !tmpStack.empty() )
    {
        _scopeStack.push(tmpStack.top());
        tmpStack.pop();
    }

    LiteralValue* copyValue = _scopes.find(ctxIndex)->second.getVariableValue(name);

    LiteralValue* castedValue = nullptr;

    if( copyValue )
        castedValue = _dataTypes.find(dataTypeId)->second->cast(copyValue);
    else
        return nullptr;

    if( castedValue == copyValue )
        return castedValue;
    else
        delete copyValue;

    return castedValue;
}

LiteralValue* Context::getArgumentValue(string name,DataTypesId dataTypeId) {
    if( !_scopes.find(_scopeStack.top())->second.exist(name) )
        return nullptr;

    LiteralValue* copyValue = _scopes.find(_scopeStack.top())->second.getVariableValue(name);

    LiteralValue* castedValue = nullptr;

    if( copyValue )
        castedValue = _dataTypes.find(dataTypeId)->second->cast(copyValue);
    else
        return nullptr;

    if( castedValue == copyValue )
        return castedValue;
    else
        delete copyValue;

    return castedValue;
}

LiteralValue* Context::getArgumentValue(string name) {
    if( !_scopes.find(_scopeStack.top())->second.exist(name) )
        return nullptr;

    return _scopes.find(_scopeStack.top())->second.getVariableValue(name);
}

bool Context::executeFunction(string name,LiteralValue* arguments) {
    if( executeVariableFunction(name,arguments) )
        return true;

    auto tmp = _functions.find(name);
    if( tmp == _functions.end() )
        return false;
    if( !get<1>(_functions.find(name)->second) )
        return false;

    get<1>(_functions.find(name)->second)->interpret(arguments);

    return true;
}

bool Context::executeVariableFunction(string name,LiteralValue* arguments) {
    auto tmp = getVariableValue(name);
    if( !tmp )
        return false;
    switch( tmp->getDataTypeId() )
    {
    case DataTypesId::Sound:
        executeMethod(name,"constantFreq",arguments);
        break;
    default:
        // TODO: Must throw?
        return false;
    }
    return true;
}

Context* Context::getInstance() {
    if ( !_instance )
        _instance = new Context();
    return _instance;
}

BaseFunction* Context::newBaseFunction(string name,DataTypesId dataType) {
    auto tmp = _functions.find(name);
    if( tmp != _functions.end() )
    {
        delete get<1>(tmp->second);
        _functions.erase(name);
    }
    BaseFunction* ret = new BaseFunction();
    _functions.insert(
        {
            name,
            tuple<DataTypesId,FunctionDefinition*>(
                dataType,
                ret)
        });
    return ret;
}

void Context::removeScope(scope_index ctx) {
    if( ctx == GLOBAL_SCOPE )
        return;
    if( _scopes.find(ctx) == _scopes.end() )
        return;

    _scopes.erase(ctx);

    if( _scopeStack.top() == ctx )
    {
        _scopeStack.pop();
        _freeScopesIndexes.push(ctx);
    }
}

bool Context::setVariableValue(string name,LiteralValue* value) {
    stack<scope_index> tmpStack;
    while( !_scopeStack.empty() )
    {
        if( !_scopes.find(_scopeStack.top())->second.exist(name) )
        {
            tmpStack.push(_scopeStack.top());
            _scopeStack.pop();
        }
        else
            break;
    }
    if( _scopeStack.empty() )
    {
        while( !tmpStack.empty() )
        {
            _scopeStack.push(tmpStack.top());
            tmpStack.pop();
        }
        return false;
    }
    size_t ctxIndex = _scopeStack.top();
    while( !tmpStack.empty() )
    {
        _scopeStack.push(tmpStack.top());
        tmpStack.pop();
    }

    DataTypesId dataType = _scopes.find(ctxIndex)->second.getDefinedDataType(name);

    LiteralValue* castedValue = nullptr;
    if( value )
    {
        castedValue = _dataTypes.find(dataType)->second->cast(value);
        if( castedValue == value )
            castedValue = value->clone();
    }

    return _scopes.find(ctxIndex)->second.setVariableValue(name,castedValue);
}

void Context::setReturnValue(LiteralValue* value) {
    if( _returnValue )
        delete _returnValue;

    if( value )
        _returnValue = value->clone();
    else
        _returnValue = value;
}

void Context::setReturnValue() {
    if( _returnValue )
        delete _returnValue;
    _returnValue = new NullLiteralValue;
}

LiteralValue* Context::getReturnValue() const {
    if( _returnValue )
        return _returnValue->clone();
    return nullptr;
}

bool Context::existReturnValue() const {
    if( _returnValue )
        return true;
    return false;
}

const variables_map Context::getAllPlayables() {
    variables_map ret;

    for( auto&& scope : _scopes )
    {
        variables_map tmp = scope.second.getAllPlayables();
        ret.insert(tmp.begin(),tmp.end());
    }

    return ret;
}
