#include "context.h"

using namespace CS;

Context* Context::_instance = nullptr;

Context::Context()
{
    _dataTypes = {
        {"sample","sample"},
        {"sound","sound"},
        {"real","real"},
        {"integer","integer"},
        {"argument","argument"},
        {"group","group"},
        {"boolean","boolean"},
        {"buffer","buffer"},
        {"string","string"},
    };

    _reservedKeywords = {
        "null",
        "sample",
        "sound",
        "real",
        "integer",
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

    _variableNames = {
    };
}

bool Context::isDataType(string name){
    return _dataTypes.find(name) != _dataTypes.end();
}

bool Context::isValidName(string name){
    return _reservedKeywords.find(name) != _reservedKeywords.end();
}

bool Context::nameExist(string name){
    return _variableNames.find(name) != _variableNames.end();
}

Context* Context::getInstance()
{
    if ( !_instance ) _instance = new Context();
    return _instance;
}
