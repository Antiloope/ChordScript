#ifndef FORINSTRUCTIONEXPRESSION_H
#define FORINSTRUCTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

class LinkedValue;
class AssignationExpression;

class ForInstructionExpression : public NonTerminalExpression {
public:
    ForInstructionExpression(size_t);
    ~ForInstructionExpression();
    void load(std::list<TerminalExpression*>*) override;
    void interpret() override;
private:
    size_t _context;
    size_t _runningContext;
    AssignationExpression* _assignation = nullptr;
    LinkedValue* _booleanOperation = nullptr;
    AssignationExpression* _endAssignation = nullptr;
    ProgramExpression* _function = nullptr;
};

}

#endif // FORINSTRUCTIONEXPRESSION_H
