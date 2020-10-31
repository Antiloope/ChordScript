#ifndef ASSIGNATIONEXPRESSION_H
#define ASSIGNATIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

class LinkedValue;
/**
 * @brief This class represents a value assignation.
 */
class AssignationExpression : public NonTerminalExpression {
public:
    AssignationExpression(size_t codeReference);
    ~AssignationExpression();
    /**
     * @brief It loads an internal LinkedValue with the tokens received
     * @param terminalExpressionsList A list of tokens
     */
    void load(std::list<TerminalExpression*>* terminalExpressionsList) override;
    /**
     * @brief It gets the LiteralValue from the stored LinkedValue
     */
    void interpret() override;
private:
    std::string _varName;
    LinkedValue* _value = nullptr;
};

}

#endif // ASSIGNATIONEXPRESSION_H
