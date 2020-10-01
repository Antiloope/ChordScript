#ifndef EOEINSTRUCTIONEXPRESSION_H
#define EOEINSTRUCTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

class EOEInstructionExpression : public NonTerminalExpression {
public:
    EOEInstructionExpression(size_t);
    ~EOEInstructionExpression();
    void load(std::list<TerminalExpression*>*) override;
    void interpret() override;
};

}

#endif // EOEINSTRUCTIONEXPRESSION_H
