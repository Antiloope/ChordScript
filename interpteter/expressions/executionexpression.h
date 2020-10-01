#ifndef EXECUTIONEXPRESSION_H
#define EXECUTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

class ArrayLinkedValue;

class ExecutionExpression : public NonTerminalExpression {
public:
    ExecutionExpression(size_t);
    ~ExecutionExpression();
    void load(std::list<TerminalExpression*>*) override;
    void interpret() override;
private:
    std::string _name;
    std::list<std::tuple<std::string,ArrayLinkedValue*>> _methodsList;
};

}

#endif // EXECUTIONEXPRESSION_H
