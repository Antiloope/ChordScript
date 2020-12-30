#include "interpreter.h"
#include "interpreter/expressions/nonterminalexpression.h"
#include "interpreter/expressions/terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/lexer.h"

using namespace std;
using namespace CS;

void Interpreter::interpret(const string sourceCode) {
    list<TerminalExpression*> expressionsList;
    ProgramExpression program;

    try
    {
        // Get the tokens
        expressionsList = Lexer::tokenize(sourceCode);

        // Send tokens to a base program expression to interpret
        program.interpret(&expressionsList);
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
