#ifndef FUNCTIONDEFINITION_H
#define FUNCTIONDEFINITION_H

#include <list>
#include "nonterminalexpressions.h"

using namespace std;

namespace CS {

class ArgumentDefinition {
public:
    ArgumentDefinition(string,string);
};

class FunctionDefinition
{
public:
    FunctionDefinition(list<TerminalExpression*>*);
private:
    list<ArgumentDefinition> _argumentsDefinitionList;
    ProgramExpression* _function;
    string _returnDataType;
    LinkedValue* _returnValue;
};

}

#endif // FUNCTIONDEFINITION_H
