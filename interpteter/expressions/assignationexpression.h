#ifndef ASSIGNATIONEXPRESSION_H
#define ASSIGNATIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

class LinkedValue;

class AssignationExpression : public NonTerminalExpression {
public:
    AssignationExpression(size_t);
    ~AssignationExpression();
    void load(std::list<TerminalExpression*>*) override;
    void interpret() override;
private:
    std::string _varName;
    LinkedValue* _value = nullptr;
};

}

#endif // ASSIGNATIONEXPRESSION_H
