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
    DataTypesId getDataTypeId() const;
private:
    DataTypesId _dataType;
    string _name;
};

class FunctionDefinition
{
public:
    FunctionDefinition();
    virtual ~FunctionDefinition();
    void load(list<TerminalExpression*>*);
    virtual void interpret(LiteralValue*);
protected:
    list<ArgumentDefinition> _argumentsDefinitionList;
    ProgramExpression* _function;
    size_t _context;
    size_t _runningContext;
};

class BaseFunction : public FunctionDefinition
{
public:
    BaseFunction();
    ~BaseFunction();
    void load(list<ArgumentDefinition>,void (*)());
    void interpret(LiteralValue*) override;
private:
    void (*_functionPointer)();
};

}

#endif // FUNCTIONDEFINITION_H
