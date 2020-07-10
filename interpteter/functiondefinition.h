#ifndef FUNCTIONDEFINITION_H
#define FUNCTIONDEFINITION_H

#include <list>
#include "nonterminalexpressions.h"

using namespace std;

namespace CS {

class ArgumentDefinition {
public:
    ArgumentDefinition(string,string,bool);
private:
    string _dataType;
    string _name;
    bool _byReference;
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
