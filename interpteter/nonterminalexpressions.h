#ifndef NONTERMINALEXPRESSION_H
#define NONTERMINALEXPRESSION_H

#include "terminalexpressions.h"

namespace CS {

class NonTerminalExpression : public Expression {
public:
    NonTerminalExpression();
    NonTerminalExpression(size_t codeReference);
    virtual ~NonTerminalExpression();
    virtual void interpret() = 0;
};

class InstructionExpression : public NonTerminalExpression {
public:
    InstructionExpression(list<TerminalExpression*>* expressionsList);
    InstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference);
    ~InstructionExpression();
    void interpret() override;
};

class ProgramExpression : public NonTerminalExpression {
public:
    ProgramExpression(list<TerminalExpression*> expressionsList);
    ProgramExpression(list<TerminalExpression*>* expressionsList);
    ProgramExpression(list<TerminalExpression*>* expressionsList, size_t codeReference);
    ~ProgramExpression();
    void interpret() override;
private:
    list<InstructionExpression*> instructionsList;
};

}

#endif // NONTERMINALEXPRESSION_H
