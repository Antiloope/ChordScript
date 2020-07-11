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
    virtual void load(list<TerminalExpression*>*) = 0;
    virtual void interpret() = 0;
};

class DefinitionExpression : public NonTerminalExpression {
public:
    DefinitionExpression(size_t);
    ~DefinitionExpression();
    void load(list<TerminalExpression*>*) override;
    void interpret() override;
};

class AssignationExpression : public NonTerminalExpression {
public:
    AssignationExpression(size_t);
    ~AssignationExpression();
    void load(list<TerminalExpression*>*) override;
    void interpret() override;
private:
    string _varName;
    LinkedValue* _value;
};

class ExecutionExpression : public NonTerminalExpression {
public:
    ExecutionExpression(size_t);
    ~ExecutionExpression();
    void load(list<TerminalExpression*>*) override;
    void interpret() override;
private:
    string _name;
    list<tuple<string,ArrayLinkedValue*>> _methodsList;
};

class ProgramExpression;

class ForInstructionExpression : public NonTerminalExpression {
public:
    ForInstructionExpression(size_t);
    ~ForInstructionExpression();
    void load(list<TerminalExpression*>*) override;
    void interpret() override;
private:
    size_t _context;
    AssignationExpression* _assignation;
    LinkedValue* _booleanOperation;
    AssignationExpression* _endAssignation;
    ProgramExpression* _function;
};

class IfInstructionExpression : public NonTerminalExpression {
public:
    IfInstructionExpression(size_t);
    ~IfInstructionExpression();
    void load(list<TerminalExpression*>*) override;
    void interpret() override;
private:
    size_t _context;
    size_t _elseContext;
    LinkedValue* _condition;
    ProgramExpression* _function;
    ProgramExpression* _elseFunction;
};

class BreakInstructionExpression : public NonTerminalExpression {
public:
    BreakInstructionExpression(size_t);
    ~BreakInstructionExpression();
    void load(list<TerminalExpression*>*) override;
    void interpret() override;
};

class EOEInstructionExpression : public NonTerminalExpression {
public:
    EOEInstructionExpression(size_t);
    ~EOEInstructionExpression();
    void load(list<TerminalExpression*>*) override;
    void interpret() override;
};

class ReturnInstructionExpression : public NonTerminalExpression {
public:
    ReturnInstructionExpression(size_t);
    ~ReturnInstructionExpression();
    void load(list<TerminalExpression*>*) override;
    void interpret() override;
private:
    LinkedValue* _returnValue = nullptr;
};

class InstructionExpression : public NonTerminalExpression {
public:
    InstructionExpression(size_t);
    InstructionExpression(const InstructionExpression&);
    ~InstructionExpression();
    void load(list<TerminalExpression*>*) override;
    void interpret() override;
private:
    NonTerminalExpression* _instruction = nullptr;
};

class ProgramExpression : public NonTerminalExpression {
public:
    ProgramExpression(size_t);
    ProgramExpression();
    ~ProgramExpression();
    void load(list<TerminalExpression*>*) override;
    void interpret() override;
    void interpret(list<TerminalExpression*>);
private:
    list<InstructionExpression*> _instructionsList;
};

}

#endif // NONTERMINALEXPRESSION_H
