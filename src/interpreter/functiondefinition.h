#ifndef FUNCTIONDEFINITION_H
#define FUNCTIONDEFINITION_H

#include <list>
#include <string>

#include "interpreter/datatypes/datatype.h"

namespace CS {

class TerminalExpression;
class ProgramExpression;

class ArgumentDefinition {
public:
    ArgumentDefinition(DataTypesId,std::string);
    std::string getName() const;
    DataTypesId getDataTypeId() const;
private:
    DataTypesId _dataType;
    std::string _name;
};

class FunctionDefinition
{
public:
    FunctionDefinition();
    virtual ~FunctionDefinition();
    void load(std::list<TerminalExpression*>*);
    virtual void interpret(LiteralValue*);
protected:
    std::list<ArgumentDefinition> _argumentsDefinitionList;
    ProgramExpression* _function;
    size_t _context;
    size_t _runningContext;
};

class BaseFunction : public FunctionDefinition
{
public:
    BaseFunction();
    ~BaseFunction();
    void load(std::list<ArgumentDefinition>,void (*)());
    void interpret(LiteralValue*) override;
private:
    void (*_functionPointer)();
};

}

#endif // FUNCTIONDEFINITION_H
