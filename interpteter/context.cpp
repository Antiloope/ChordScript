#include "context.h"

using namespace CS;

Context* Context::_instance = nullptr;

Context::Context()
{
    _dataTypes = {
        {"sample",      new SampleDataType()},
        {"sound",       new SoundDataType()},
        {"numeric",     new NumericDataType()},
        {"argument",    new ArgumentDataType()},
        {"group",       new GroupDataType()},
        {"boolean",     new BooleanDataType()},
        {"buffer",      new BufferDataType()},
        {"string",      new StringDataType()},
        {"null",        new NullDataType()},
        {"function",    new FunctionDataType()},
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

    _currentContext = GlobalContext;
}

size_t Context::newContext(){
    static size_t counter;
    _variables.insert(pair<size_t,variables_map>(++counter,variables_map()));
    _currentContext = counter;
    return counter;
}

size_t Context::switchContext(size_t ctx){
    if ( _variables.find(ctx) == _variables.end() ) return 0;
    _currentContext = ctx;
    return ctx;
}

size_t Context::getCurrentContext() const {
    return _currentContext;
}

bool Context::isDataType(string name){
    return _dataTypes.find(name) != _dataTypes.end();
}

bool Context::isValidName(string name){
    return _reservedKeywords.find(name) == _reservedKeywords.end();
}

string Context::getDataTypeName(string name){
    if(_variables.find(_currentContext)->second.find(name) != _variables.find(_currentContext)->second.end())
    {
        return get<1>(_variables.find(_currentContext)->second.find(name)->second)->getDataTypeName();
    }
    return get<1>(_variables.find(GlobalContext)->second.find(name)->second)->getDataTypeName();
}

bool Context::nameExist(string name){
    return
        (_variables.find(_currentContext)->second.find(name) != _variables.find(_currentContext)->second.end() ||
         _variables.find(GlobalContext)->second.find(name) != _variables.find(GlobalContext)->second.end() );
}

bool Context::functionNameExist(string name){
    return _functions.find(name) != _functions.end();
}

void Context::newVariable(string name, string dataType, LinkedValue* value){
    auto tmp = _variables.find(_currentContext)->second.find(name);
    if(tmp != _variables.find(_currentContext)->second.end())
    {
        delete get<1>(tmp->second);
        delete get<2>(tmp->second);
        _variables.find(_currentContext)->second.erase(name);
    }
    _variables.find(_currentContext)->second.insert({name,tuple<string,LinkedValue*,LiteralValue*>(dataType,value,nullptr)});
}

#include "functiondefinition.h"

void Context::newFunction(string name, string dataType ,FunctionDefinition* function){
    auto tmp = _functions.find(name);
    if(tmp != _functions.end())
    {
        delete get<1>(tmp->second);
        _functions.erase(name);
    }
    _functions.insert({name,tuple<string,FunctionDefinition*>(dataType,function)});
}

Context* Context::getInstance()
{
    if ( !_instance ) _instance = new Context();
    return _instance;
}
