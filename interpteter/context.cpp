#include "context.h"
#include "functiondefinition.h"
#include "languagedefinitions.h"

using namespace std;
using namespace CS;

Context* Context::_instance = nullptr;

Scope::Scope() {}

Scope::Scope(const Scope& scope) {
    for( auto var : scope._scope ) {
        get<1>(get<1>(var)) = get<1>(get<1>(var))->clone();
        _scope.insert(var);
    }
}

Scope::~Scope() {
    for(auto it : _scope)
    {
        if (get<1>(it.second)) delete get<1>(it.second);
    }
}

bool Scope::exist(string name) {
    if( _scope.find(name) != _scope.end() ) return true;
    return false;
}

DataTypesId Scope::getVariableDataType(string name) {
    if( !get<1>(_scope.find(name)->second) ) return DataTypesId::Null;
    return get<1>(_scope.find(name)->second)->getDataTypeId();
}

DataTypesId Scope::getDefinedDataType(string name) {
    return get<0>(_scope.find(name)->second);
}

void Scope::newVariable(string name, DataTypesId dataType) {
    if( _scope.find(name) != _scope.end() )
    {
        if( get<1>(_scope.find(name)->second) ) delete get<1>(_scope.find(name)->second);
        _scope.erase(name);
    }
    _scope.insert({name,tuple<DataTypesId,LiteralValue*>(dataType,new NullLiteralValue())});
}

LiteralValue* Scope::getVariableValue(string name) {
    return get<1>(_scope.find(name)->second);
}

bool Scope::setVariableValue(string name,LiteralValue* value) {
    if( _scope.find(name) == _scope.end() ) return false;
    if( get<1>(_scope.find(name)->second) )
        delete get<1>(_scope.find(name)->second);
    get<1>(_scope.find(name)->second) = value;
    return true;
}

Context::Context() {
    _dataTypes = {
        {DataTypesId::Sample,      new SampleDataType()},
        {DataTypesId::Sound,       new SoundDataType()},
        {DataTypesId::Numeric,     new NumericDataType()},
        {DataTypesId::Argument,    new ArgumentDataType()},
        {DataTypesId::Group,       new GroupDataType()},
        {DataTypesId::Boolean,     new BooleanDataType()},
        {DataTypesId::Buffer,      new BufferDataType()},
        {DataTypesId::String,      new StringDataType()},
        {DataTypesId::Function,    new FunctionDataType()},
    };

    _scopes.insert( pair<size_t,Scope>(GlobalScope,Scope()) );

    _functions = {};

    _scopeStack.push(GlobalScope);

    _returnValue = nullptr;

    for(scope_index i = MaxContextCount ; i != 0 ; i--)
    {
        _freeScopesIndexes.push(i);
    }
}

void Context::load() {

    list<ArgumentDefinition> argumentsList = {
        ArgumentDefinition(DataTypesId::Numeric,Functions::freqParam),
        ArgumentDefinition(DataTypesId::Numeric,Functions::durationParam)
    };

    BaseFunction* tmpFunction = this->newBaseFunction(Names::sinFunction,DataTypesId::Buffer);
    tmpFunction->load(argumentsList,&Functions::sin);

    tmpFunction = this->newBaseFunction(Names::squareFunction,DataTypesId::Buffer);
    tmpFunction->load(argumentsList,&Functions::square);

    this->newVariable(Names::sinSound,DataTypesId::Sound);
    this->newVariable(Names::sinSound,DataTypesId::Sound);

    LiteralValue* value = new SoundLiteralValue(new SoundGenerator(Names::sinFunction));
    _scopes.find(GlobalScope)->second.setVariableValue(Names::sinSound,value);
    value = new SoundLiteralValue(new SoundGenerator(Names::squareFunction));
    _scopes.find(GlobalScope)->second.setVariableValue(Names::squareSound,value);


    this->newVariable(Names::a3,DataTypesId::Numeric);
    this->newVariable(Names::c3,DataTypesId::Numeric);

    _scopes.find(GlobalScope)->second.setVariableValue(Names::a3,new NumericLiteralValue(220));
    _scopes.find(GlobalScope)->second.setVariableValue(Names::c3,new NumericLiteralValue(130.813));
}

scope_index Context::newScope() {
    scope_index ctxId = _freeScopesIndexes.top();
    _freeScopesIndexes.pop();
    _scopes.insert(pair<scope_index,Scope>(ctxId,Scope()));
    _scopeStack.push(ctxId);
    return ctxId;
}

void Context::returnScope() {
    if ( _scopeStack.top() ) _scopeStack.pop();
}

scope_index Context::switchScope(scope_index scopeIndex) {
    if( _scopes.find(scopeIndex) == _scopes.end() ) return GlobalScope;
    scope_index returnScopeId = _freeScopesIndexes.top();
    _freeScopesIndexes.pop();
    _scopes.insert(pair<scope_index,Scope>(returnScopeId,Scope(_scopes.find(scopeIndex)->second)));

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
    return constants::isValidName(name);
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
    _functions.insert({name,tuple<DataTypesId,FunctionDefinition*>(dataType,ret)});
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
    LiteralValue* literalValue = _scopes.find(_scopeStack.top())->second.getVariableValue(variableName);
    while( !tmpStack.empty() )
    {
        _scopeStack.push(tmpStack.top());
        tmpStack.pop();
    }

    DataType* dataType = _dataTypes.find(dataTypeId)->second;

    if( !dataType->executeMethod(methodName,literalValue,arguments) ) return false;

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
        else break;
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

    return _scopes.find(ctx)->second.getVariableValue(name)->clone();
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
        else break;
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

    LiteralValue* copyValue = _scopes.find(ctxIndex)->second.getVariableValue(name)->clone();

    LiteralValue* castedValue = _dataTypes.find(dataTypeId)->second->cast(copyValue);

    if( !castedValue ) delete copyValue;
    return castedValue;
}

LiteralValue* Context::getArgumentValue(string name,DataTypesId dataTypeId) {
    if( !_scopes.find(_scopeStack.top())->second.exist(name) ) return nullptr;

    LiteralValue* copyValue = _scopes.find(_scopeStack.top())->second.getVariableValue(name)->clone();

    LiteralValue* castedValue = _dataTypes.find(dataTypeId)->second->cast(copyValue);

    if( !castedValue ) delete copyValue;
    return castedValue;
}

LiteralValue* Context::getArgumentValue(string name) {
    if( !_scopes.find(_scopeStack.top())->second.exist(name) ) return nullptr;

    return _scopes.find(_scopeStack.top())->second.getVariableValue(name)->clone();
}

bool Context::executeFunction(string name,LiteralValue* arguments) {
    auto tmp = _functions.find(name);
    if( tmp == _functions.end() ) return false;
    if( !get<1>(_functions.find(name)->second) ) return false;

    get<1>(_functions.find(name)->second)->interpret(arguments);

    return true;
}

Context* Context::getInstance() {
    if ( !_instance ) _instance = new Context();
    return _instance;
}

BaseFunction* Context::newBaseFunction(string name,DataTypesId dataType) {
    auto tmp = _functions.find(name);
    if(tmp != _functions.end())
    {
        delete get<1>(tmp->second);
        _functions.erase(name);
    }
    BaseFunction* ret = new BaseFunction();
    _functions.insert({name,tuple<DataTypesId,FunctionDefinition*>(dataType,ret)});
    return ret;
}

void Context::removeScope(scope_index ctx) {
    if( ctx == GlobalScope ) return;
    if( _scopes.find(ctx) == _scopes.end() ) return;

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
        else break;
    }
    if( _scopeStack.empty() )
    {
        while( !tmpStack.empty() )
        {
            _scopeStack.push(tmpStack.top());
            tmpStack.pop();
        }
        if( value ) delete value;
        return false;
    }
    size_t ctxIndex = _scopeStack.top();
    while( !tmpStack.empty() )
    {
        _scopeStack.push(tmpStack.top());
        tmpStack.pop();
    }

    DataTypesId dataType = _scopes.find(ctxIndex)->second.getDefinedDataType(name);

    LiteralValue* castedValue = _dataTypes.find(dataType)->second->cast(value);

    return _scopes.find(ctxIndex)->second.setVariableValue(name,castedValue);
}

void Context::setReturnValue(LiteralValue* value) {
    if( value == _returnValue )
    {
        delete value;
        return;
    }
    if( _returnValue ) delete _returnValue;
    _returnValue = value;
}

LiteralValue* Context::getReturnValue() const {
    return _returnValue;
}
