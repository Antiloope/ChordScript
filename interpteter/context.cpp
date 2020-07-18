#include "context.h"

using namespace CS;

Context* Context::_instance = nullptr;

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

    _reservedKeywords = {
        "null",
        "sample",
        "sound",
        "numeric",
        "argument",
        "group",
        "boolean",
        "buffer",
        "string",
        "function",
        "for",
        "if",
        "break",
        "return",
        "else",
        "true",
        "false",
    };

    _variables.insert( pair<size_t,variables_map>(GlobalContext,variables_map()) );

    _functions = {
    };

    _contextStack.push(GlobalContext);

    _returnValue = nullptr;
}

context_index Context::newContext() {
    static context_index counter;
    _variables.insert(pair<context_index,variables_map>(++counter,variables_map()));
    _contextStack.push(counter);
    return counter;
}

void Context::returnContext() {
    if ( _contextStack.top() ) _contextStack.pop();
}

bool Context::switchContext(context_index contextIndex) {
    if( _variables.find(contextIndex) == _variables.end() ) return false;
    _contextStack.push(contextIndex);
    return true;
}

context_index Context::getCurrentContext() const {
    return _contextStack.top();
}

bool Context::isDataType(DataTypesId dataType) const {
    return _dataTypes.find(dataType) != _dataTypes.end();
}

bool Context::isValidName(string name) const {
    return _reservedKeywords.find(name) == _reservedKeywords.end();
}

DataTypesId Context::getDataTypeId(string name) {
    stack<context_index> tmpStack;
    while( !_contextStack.empty() )
    {
        if( _variables.find(_contextStack.top())->second.find(name) == _variables.find(_contextStack.top())->second.end() )
        {
            tmpStack.push(_contextStack.top());
            _contextStack.pop();
        }
        else break;
    }
    if( _contextStack.empty() )
    {
        while( !tmpStack.empty() )
        {
            _contextStack.push(tmpStack.top());
            tmpStack.pop();
        }
        return DataTypesId::Null;
    }
    context_index currentContext = _contextStack.top();
    while( !tmpStack.empty() )
    {
        _contextStack.push(tmpStack.top());
        tmpStack.pop();
    }

    if( get<1>(_variables.find(currentContext)->second.find(name)->second) )
        return get<1>(_variables.find(currentContext)->second.find(name)->second)->getDataTypeId();
    return DataTypesId::Null;
}

bool Context::nameExist(string name) {
    stack<context_index> tmpStack;
    while( !_contextStack.empty() )
    {
        if( _variables.find(_contextStack.top())->second.find(name) == _variables.find(_contextStack.top())->second.end() )
        {
            tmpStack.push(_contextStack.top());
            _contextStack.pop();
        }
        else break;
    }
    if( _contextStack.empty() )
    {
        while( !tmpStack.empty() )
        {
            _contextStack.push(tmpStack.top());
            tmpStack.pop();
        }
        return false;
    }
    while( !tmpStack.empty() )
    {
        _contextStack.push(tmpStack.top());
        tmpStack.pop();
    }
    return true;
}

bool Context::functionNameExist(string name) const {
    return _functions.find(name) != _functions.end();
}

void Context::newVariable(string name, DataTypesId dataType) {
    context_index currentContext = _contextStack.top();
    auto tmp = _variables.find(currentContext)->second.find(name);
    if(tmp != _variables.find(currentContext)->second.end())
    {
        delete get<1>(tmp->second);
        _variables.find(currentContext)->second.erase(name);
    }
    _variables.find(currentContext)->second.insert({name,tuple<DataTypesId,LiteralValue*>(dataType,nullptr)});
}

#include "functiondefinition.h"

void Context::newFunction(string name, DataTypesId dataType ,FunctionDefinition* function) {
    auto tmp = _functions.find(name);
    if(tmp != _functions.end())
    {
        delete get<1>(tmp->second);
        _functions.erase(name);
    }
    _functions.insert({name,tuple<DataTypesId,FunctionDefinition*>(dataType,function)});
}

bool Context::executeMethod(string variableName,string methodName,LiteralValue* arguments) {
    stack<context_index> tmpStack;
    while( !_contextStack.empty() )
    {
        if( _variables.find(_contextStack.top())->second.find(variableName) == _variables.find(_contextStack.top())->second.end() )
        {
            tmpStack.push(_contextStack.top());
            _contextStack.pop();
        }
        else break;
    }
    if( _contextStack.empty() )
    {
        while( !tmpStack.empty() )
        {
            _contextStack.push(tmpStack.top());
            tmpStack.pop();
        }
        return false;
    }
    DataTypesId dataTypeId = get<0>(_variables.find(_contextStack.top())->second.find(variableName)->second);
    LiteralValue* literalValue = get<1>(_variables.find(_contextStack.top())->second.find(variableName)->second);
    while( !tmpStack.empty() )
    {
        _contextStack.push(tmpStack.top());
        tmpStack.pop();
    }

    DataType* dataType = _dataTypes.find(dataTypeId)->second;

    dataType->executeMethod(methodName,literalValue,arguments);

    return true;
}

LiteralValue* Context::getVariableValue(string name) {
    stack<context_index> tmpStack;
    while( !_contextStack.empty() )
    {
        if( _variables.find(_contextStack.top())->second.find(name) == _variables.find(_contextStack.top())->second.end() )
        {
            tmpStack.push(_contextStack.top());
            _contextStack.pop();
        }
        else break;
    }
    if( _contextStack.empty() )
    {
        while( !tmpStack.empty() )
        {
            _contextStack.push(tmpStack.top());
            tmpStack.pop();
        }
        return nullptr;
    }
    size_t ctx = _contextStack.top();
    while( !tmpStack.empty() )
    {
        _contextStack.push(tmpStack.top());
        tmpStack.pop();
    }

    return get<1>(_variables.find(ctx)->second.find(name)->second)->clone();
}

FunctionDefinition* Context::getFunction(string name) {
    auto tmp = _functions.find(name);
    if( tmp == _functions.end() ) return nullptr;
    return get<1>(_functions.find(name)->second);
}

Context* Context::getInstance() {
    if ( !_instance ) _instance = new Context();
    return _instance;
}

void Context::removeContext(context_index ctx) {
    auto tmp = _variables.find(ctx);
    if( tmp == _variables.end() ) return;

    variables_map variableMap = tmp->second;
    for(auto it : variableMap)
    {
        if (get<1>(it.second)) delete get<1>(it.second);
    }

    _variables.erase(ctx);
}

bool Context::setVariableValue(string name,LiteralValue* value) {
    stack<context_index> tmpStack;
    while( !_contextStack.empty() )
    {
        if( _variables.find(_contextStack.top())->second.find(name) == _variables.find(_contextStack.top())->second.end() )
        {
            tmpStack.push(_contextStack.top());
            _contextStack.pop();
        }
        else break;
    }
    if( _contextStack.empty() )
    {
        while( !tmpStack.empty() )
        {
            _contextStack.push(tmpStack.top());
            tmpStack.pop();
        }
        return false;
    }
    size_t ctxIndex = _contextStack.top();
    while( !tmpStack.empty() )
    {
        _contextStack.push(tmpStack.top());
        tmpStack.pop();
    }

    DataTypesId dataType = get<0>(_variables.find(ctxIndex)->second.find(name)->second);

    LiteralValue* castedValue = _dataTypes.find(dataType)->second->cast(value);

    if( get<1>(_variables.find(ctxIndex)->second.find(name)->second) )
        delete get<1>(_variables.find(ctxIndex)->second.find(name)->second);
    get<1>(_variables.find(ctxIndex)->second.find(name)->second) = castedValue;
    return true;
}

void Context::setReturnValue(LiteralValue* value) {
    if( _returnValue ) delete _returnValue;
    _returnValue = value;
}

LiteralValue* Context::getReturnValue() const {
    return _returnValue;
}
