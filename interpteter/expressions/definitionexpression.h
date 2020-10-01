#ifndef DEFINITIONEXPRESSION_H
#define DEFINITIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

class DefinitionExpression : public NonTerminalExpression {
public:
    DefinitionExpression(size_t);
    ~DefinitionExpression();
    void load(std::list<TerminalExpression*>*) override;
    void interpret() override;
};

}

#endif // DEFINITIONEXPRESSION_H
