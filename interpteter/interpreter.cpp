#include "interpreter.h"
#include "lexer.h"
#include "utils/Exceptions/exception.h"
#include "nonterminalexpressions.h"

using namespace std;
using namespace CS;

Interpreter::Interpreter()
{

}

void Interpreter::interpret(const string sourceCode)
{
    Lexer lexer;
    list<TerminalExpression*> expressionsList;
    try {
        expressionsList = lexer.tokenize(sourceCode);
    } catch (const SyntaxException& e) {
        throw e;
    } catch (const exception& e) {
        throw e;
    }

    ProgramExpression* program;
    try {
        program = new ProgramExpression(expressionsList);
    } catch (const SyntaxException& e) {
        throw e;
    } catch (const SemanticException& e) {
        throw e;
    } catch (const exception& e) {
        throw e;
    }

    try {
        program->interpret();
    } catch (const SyntaxException& e) {
        throw e;
    } catch (const exception& e) {
        throw e;
    }

    return;
}
