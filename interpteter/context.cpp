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
}

bool Context::isDataType(string name){
    return _dataTypes.find(name) != _dataTypes.end();
}

bool Context::isValidName(string name){
    return _reservedKeywords.find(name) == _reservedKeywords.end();
}

string Context::getDataTypeName(string name){
    return _variables.find(name)->second->getDataTypeName();
}

bool Context::nameExist(string name){
    return _variables.find(name) != _variables.end();
}

void Context::newVariable(string name, Value* value){
    auto tmp = _variables.find(name);
    if(tmp != _variables.end())
    {
        delete tmp->second;
        _variables.erase(name);
    }
    _variables.insert({name,value});
}

Context* Context::getInstance()
{
    if ( !_instance ) _instance = new Context();
    return _instance;
}
