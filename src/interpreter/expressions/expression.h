#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

namespace CS {

/**
 * @brief Base class for expressions. It implements a common codeReference storage
 * See NonTerminalExpression and TerminalExpression
 */
class Expression {
public:
    Expression();
    Expression(size_t codeReference); // Set a code reference
    virtual ~Expression();
    size_t getCodeReference() const;
private:
    size_t _codeReference;
};

}

#endif // EXPRESSION_H
