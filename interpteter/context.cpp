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
        {"null",        new DataType()},
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
        "for",
        "if",
        "break",
        "return",
        "else",
        "true",
        "false",
    };

    _variables[GlobalContext] = variables_map();

    _functions = {
    };

    _currentContext = GlobalContext;
}

bool Context::isDataType(string name){
    return _dataTypes.find(name) != _dataTypes.end();
}

bool Context::isValidName(string name){
    return _reservedKeywords.find(name) == _reservedKeywords.end();
}

string Context::getDataTypeName(string name){
    if(_variables[_currentContext].find(name) != _variables[_currentContext].end())
    {
        return get<1>(_variables[_currentContext].find(name)->second)->getDataTypeName();
    }
    return get<1>(_variables[GlobalContext].find(name)->second)->getDataTypeName();
}

bool Context::nameExist(string name){
    return
        (_variables[_currentContext].find(name) != _variables[_currentContext].end() ||
         _variables[GlobalContext].find(name) != _variables[GlobalContext].end() );
}

bool Context::functionNameExist(string name){
    return _functions.find(name) != _functions.end();
}

void Context::newVariable(string name, string dataType, LinkedValue* value){
    auto tmp = _variables[_currentContext].find(name);
    if(tmp != _variables[_currentContext].end())
    {
        delete get<1>(tmp->second);
        delete get<2>(tmp->second);
        _variables[_currentContext].erase(name);
    }
    _variables[_currentContext].insert({name,tuple<string,LinkedValue*,LiteralValue*>(dataType,value,nullptr)});
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
