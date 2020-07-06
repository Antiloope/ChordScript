#ifndef NONTERMINALEXPRESSION_H
#define NONTERMINALEXPRESSION_H

#include "value.h"

namespace CS {

class FunctionDefinition;

class NonTerminalExpression : public Expression {
public:
    NonTerminalExpression();
    NonTerminalExpression(size_t);
    virtual ~NonTerminalExpression();
    virtual void interpret() = 0;
};

class DefinitionExpression : public NonTerminalExpression {
public:
    DefinitionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference);
    ~DefinitionExpression();
    void interpret() override;
};

class AssignationExpression : public NonTerminalExpression {
public:
    AssignationExpression(list<TerminalExpression*>* expressionsList, size_t codeReference);
    ~AssignationExpression();
    void interpret() override;
private:
    string _varName;
    string _dataType;
};

class ExecutionExpression : public NonTerminalExpression {
public:
    ExecutionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference);
    ~ExecutionExpression();
    void interpret() override;
private:
    string _name;
    list<tuple<string,ArrayLinkedValue*>> _methodsList;
};

class ForInstructionExpression : public NonTerminalExpression {
public:
    ForInstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference);
    ~ForInstructionExpression();
    void interpret() override;
};

class IfInstructionExpression : public NonTerminalExpression {
public:
    IfInstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference);
    ~IfInstructionExpression();
    void interpret() override;
};

class BreakInstructionExpression : public NonTerminalExpression {
public:
    BreakInstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference);
    ~BreakInstructionExpression();
    void interpret() override;
};

class EOEInstructionExpression : public NonTerminalExpression {
public:
    EOEInstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference);
    ~EOEInstructionExpression();
    void interpret() override;
};

class ReturnInstructionExpression : public NonTerminalExpression {
public:
    ReturnInstructionExpression(list<TerminalExpression*>*,size_t codeReference);
    ~ReturnInstructionExpression();
    void interpret() override;
private:
    LinkedValue* _returnValue;
};

class InstructionExpression : public NonTerminalExpression {
public:
    InstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference);
    ~InstructionExpression();
    void interpret() override;
private:
    NonTerminalExpression* _instruction;
};

class ProgramExpression : public NonTerminalExpression {
public:
    ProgramExpression(list<TerminalExpression*> expressionsList);
    ProgramExpression(list<TerminalExpression*>* expressionsList, size_t codeReference);
    ~ProgramExpression();
    void interpret() override;
private:
    list<InstructionExpression*> _instructionsList;
};

}

#endif // NONTERMINALEXPRESSION_H
