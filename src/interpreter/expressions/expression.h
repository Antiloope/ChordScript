#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <list>

namespace CS {

/**
 * @brief Base class for expressions. Implement a common codeReference storage
 * See NonTerminalExpression and TerminalExpression
 */
class Expression {
public:
    Expression();
    Expression(size_t);
    virtual ~Expression();
    size_t getCodeReference() const;
private:
    size_t _codeReference;
};

}

#endif // EXPRESSION_H
