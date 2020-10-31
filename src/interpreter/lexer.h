#ifndef LEXER_H
#define LEXER_H

#include <list>
#include <string>

namespace CS {

// Defined in terminalexpressions.h
class TerminalExpression;

/**
 * @brief Has only one static method that returns the tokens
 * that are generated based on source code.
 */
class Lexer
{
public:
    /**
     * @brief Generates a list of CS::TerminalExpression based
     * on source code
     * @param sourceCode Plain text source code
     * @return An std::list of CS::TerminalExpression* that holds
     * all the information about the token (see TerminalExpression)
     */
    static std::list<TerminalExpression*> tokenize(const std::string sourceCode);
};

}

#endif // LEXER_H
