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
    list<Expression*> expressionsQueue;
    try {
        expressionsQueue = lexer.tokenize(sourceCode);
    } catch (const SyntaxException& e) {
        throw e;
    } catch (const exception& e) {
        throw e;
    }

    try {
        Expression *tmp = new Expression(0);
        tmp->interpret(&expressionsQueue);
        delete tmp;
    } catch (const SyntaxException& e) {
        throw e;
    } catch (const exception& e) {
        throw e;
    }

    return;
}
