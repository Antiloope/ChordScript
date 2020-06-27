#ifndef LEXER_H
#define LEXER_H

#include <list>
#include "terminalexpressions.h"

using namespace std;

namespace CS {

class Lexer
{
public:
    Lexer(){};
    list<TerminalExpression*> tokenize(string sourceCode);
};

}

#endif // LEXER_H
