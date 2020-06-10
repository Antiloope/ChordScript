#ifndef LEXER_H
#define LEXER_H

#include <queue>
#include "expression.h"

using namespace std;

namespace CS {

queue<Expression*> tokenize(string sourceCode);

}

#endif // LEXER_H
