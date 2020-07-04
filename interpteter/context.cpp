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

    _variables = {
    };

    _functions = {
    };
}

bool Context::isDataType(string name){
    return _dataTypes.find(name) != _dataTypes.end();
}

bool Context::isValidName(string name){
    return _reservedKeywords.find(name) == _reservedKeywords.end();
}

string Context::getDataTypeName(string name){
    return get<1>(_variables.find(name)->second)->getDataTypeName();
}

bool Context::nameExist(string name){
    return _variables.find(name) != _variables.end();
}

bool Context::functionNameExist(string name){
    return _functions.find(name) != _functions.end();
}

void Context::newVariable(string name, string dataType, LinkedValue* value){
    auto tmp = _variables.find(name);
    if(tmp != _variables.end())
    {
        delete get<1>(tmp->second);
        delete get<2>(tmp->second);
        _variables.erase(name);
    }
    _variables.insert({name,tuple<string,LinkedValue*,LiteralValue*>(dataType,value,nullptr)});
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
