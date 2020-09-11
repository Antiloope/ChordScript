#include "interpreter.h"
#include "lexer.h"
#include "utils/Exceptions/exception.h"
#include "nonterminalexpressions.h"
#include "executor/executorinterface.h"
#include "context.h"
#include "datatype.h"

using namespace std;
using namespace CS;

Interpreter::Interpreter() {

}

void Interpreter::interpret(const string sourceCode) {
    Lexer lexer;
    list<TerminalExpression*> expressionsList;
    ProgramExpression program;

    try {
        expressionsList = lexer.tokenize(sourceCode);
        program.interpret(expressionsList);
    } catch (const SyntaxException& e) {
        TerminalExpression* tmp;
        while(!expressionsList.empty())
        {
            tmp = expressionsList.front();
            expressionsList.pop_front();
            delete tmp;
        }
        throw e;
    } catch (const SemanticException& e) {
        TerminalExpression* tmp;
        while(!expressionsList.empty())
        {
            tmp = expressionsList.front();
            expressionsList.pop_front();
            delete tmp;
        }
        throw e;
    } catch (const exception& e) {
        TerminalExpression* tmp;
        while(!expressionsList.empty())
        {
            tmp = expressionsList.front();
            expressionsList.pop_front();
            delete tmp;
        }
        throw e;
    }

    return;
}
