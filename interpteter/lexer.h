#ifndef LEXER_H
#define LEXER_H

#include <list>
#include "expression.h"

using namespace std;

namespace CS {

class Lexer
{
public:
    Lexer(){};
    list<Expression*> tokenize(string sourceCode);
};

}

#endif // LEXER_H
