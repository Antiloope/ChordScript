#include "interpreter.h"
#include "expressions/nonterminalexpression.h"
#include "expressions/terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "lexer.h"

using namespace std;
using namespace CS;

void Interpreter::interpret(const string sourceCode) {
    list<TerminalExpression*> expressionsList;
    ProgramExpression program;

    try
    {
        expressionsList = Lexer::tokenize(sourceCode);
        program.interpret(expressionsList);
    }
    catch( const SyntaxException& e )
    {
        TerminalExpression* tmp;
        while( !expressionsList.empty() )
        {
            tmp = expressionsList.front();
            expressionsList.pop_front();
            delete tmp;
        }
        throw e;
    }
    catch( const SemanticException& e )
    {
        TerminalExpression* tmp;
        while( !expressionsList.empty() )
        {
            tmp = expressionsList.front();
            expressionsList.pop_front();
            delete tmp;
        }
        throw e;
    }
    catch( const exception& e )
    {
        TerminalExpression* tmp;
        while( !expressionsList.empty() )
        {
            tmp = expressionsList.front();
            expressionsList.pop_front();
            delete tmp;
        }
        throw e;
    }
}