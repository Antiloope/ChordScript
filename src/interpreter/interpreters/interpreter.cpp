#include "interpreter.h"
#include "interpreter/expressions/nonterminalexpression.h"
#include "interpreter/expressions/terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/lexer.h"
#include <stack>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
using namespace CS;

void CS::redirectOutput(RedirectOutput option) {
    static stack<pair<int,int>> saves;

    switch( option )
    {
    case SetNull:
    {
        pair<int,int> save = {
            dup(1),
            dup(2)
        };

        dup2(open("/dev/null",O_WRONLY), 1);
        dup2(open("/dev/null",O_WRONLY), 2);

        saves.push(save);
    }
        break;
    case Reset:
    {
        if( saves.empty() )
            return;

        pair<int,int> save = saves.top();

        dup2(save.first, 1);
        dup2(save.second, 2);
        close(save.first);
        close(save.second);

        saves.pop();
    }
    case Clean:
    {
        while( true )
        {
            if( saves.empty() )
                return;

            pair<int,int> save = saves.top();

            dup2(save.first, 1);
            dup2(save.second, 2);
            close(save.first);
            close(save.second);

            saves.pop();
        }
    }
        break;
    }
}

bool Interpreter::interpret(const string sourceCode) {
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
    return true;
}
