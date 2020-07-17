#ifndef FUNCTIONDEFINITION_H
#define FUNCTIONDEFINITION_H

#include <list>
#include "nonterminalexpressions.h"

using namespace std;

namespace CS {

class ArgumentDefinition {
public:
    ArgumentDefinition(DataTypesId,string);
    string getName() const;
private:
    DataTypesId _dataType;
    string _name;
};

class FunctionDefinition
{
public:
    FunctionDefinition();
    ~FunctionDefinition();
    void load(list<TerminalExpression*>*);
    void interpret(LiteralValue*);
private:
    list<ArgumentDefinition> _argumentsDefinitionList;
    ProgramExpression* _function;
    size_t _context;
};

}

#endif // FUNCTIONDEFINITION_H
