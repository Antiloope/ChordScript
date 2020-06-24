#include "interpreter.h"
#include "lexer.h"
#include "utils/Exceptions/exception.h"

using namespace std;
using namespace CS;

Interpreter::Interpreter()
{

}

void Interpreter::interpret(const string sourceCode)
{
    Lexer lexer;
    queue<Expression*> expressionsQueue;
    try {
        expressionsQueue = lexer.tokenize(sourceCode);
    } catch (const SyntaxException& e) {
        throw SyntaxException(e);
    } catch (const exception& e) {
    }
    return;
}
