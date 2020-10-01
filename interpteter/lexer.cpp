#include "lexer.h"
#include "utils/Exceptions/exception.h"
#include "utils/log.h"
#include "languageConstants.h"
#include "terminalexpressions.h"
#include <stack>

using namespace std;
using namespace CS;
using namespace CS::Constants;

list<TerminalExpression*> Lexer::tokenize(const string sourceCode){
    list<TerminalExpression*> tokens;

    // To check correct number and order of parenthesis,
    //  brackets, and braces
    stack<char> containersStack;

    size_t i = 0;
    while( i < sourceCode.length() )
    {
        switch( sourceCode[i] )
        {
            case EQUAL:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Equal)));
                break;
            case END_OF_EXPRESSION:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::EOE)));
                break;
            case SUBSTRACTION:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Substract)));
                break;
            case ADDITION:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Addition)));
                break;
            case MULTIPLICATION:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Multiplication)));
                break;
            case DIVITION:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Divition)));
                break;
            case NEGATION:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Negation)));
                break;
            case GREATER_THAN:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::GreaterThan)));
                break;
            case LESS_THAN:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::LessThan)));
                break;
            case OR:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Or)));
                break;
            case AND:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::And)));
                break;
            case MEMBER_ACCESS:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::MemberAccess)));
                break;
            case SEPARATOR:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Separator)));
                break;
            case OPEN_PARENTHESIS:
                containersStack.push('(');
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::OpenParenthesis)));
                break;
            case CLOSE_PARENTHESIS:
                containersStack.top() == '('
                    ? containersStack.pop()
                    : throw SyntaxException("Closing a wrong parenthesis", i);

                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::CloseParenthesis)));
                break;
            case OPEN_BRACKET:
                containersStack.push('[');
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::OpenBracket)));
                break;
            case CLOSE_BRACKET:
                containersStack.top() == '['
                    ? containersStack.pop()
                    : throw SyntaxException("Closing a wrong bracket", i);

                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::CloseBracket)));
                break;
            case OPEN_BRACE:
                containersStack.push('{');
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::OpenBrace)));
                break;
            case CLOSE_BRACE:
                containersStack.top() == '{'
                    ? containersStack.pop()
                    : throw SyntaxException("Closing a wrong brace", i);

                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::CloseBrace)));
                break;
            case END_OF_LINE:
            case SPACE:
                break;
            case START_COMMENT:
                i++;
                while( sourceCode[i] != END_OF_LINE )
                {
                    if( i >= sourceCode.length() )
                        break;
                    i++;
                }
                i--;
                break;
            case STIRNG_DELIMITATOR:
            {
                string tmp = "";
                size_t codeRef = i;
                i++;
                while( sourceCode[i] != STIRNG_DELIMITATOR )
                {
                    if( i >= sourceCode.length() )
                        throw SyntaxException("String delimitator never closed",i);
                    tmp.push_back(sourceCode[i]);
                    i++;
                }
                tokens.push_back(new StringExpression(codeRef,cCast(ExpressionTypes::String),tmp));
            }
                break;
            default:
            {
                if( (sourceCode[i] >= 'A' && sourceCode[i] <= 'Z') ||
                    (sourceCode[i] >= 'a' && sourceCode[i] <= 'z') ||
                    (sourceCode[i] == '_')
                    )
                {
                    string tmp = "";
                    size_t codeRef = i;
                    tmp.push_back(sourceCode[i]);
                    i++;
                    while(
                        (sourceCode[i] >= 'A' && sourceCode[i] <= 'Z') ||
                        (sourceCode[i] >= 'a' && sourceCode[i] <= 'z') ||
                        (sourceCode[i] == '_') ||
                        (sourceCode[i] >= '0' && sourceCode[i] <= '9')
                        )
                    {
                        tmp.push_back(sourceCode[i]);
                        i++;
                    }
                    // Check for reseved keywords
                    if( tmp == FOR )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::For)));
                    else if( tmp == IF )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::If)));
                    else if( tmp == ELSE )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::Else)));
                    else if( tmp == BREAK )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::Break)));
                    else if( tmp == RETURN )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::Return)));
                    else if( tmp == TRUE )
                        tokens.push_back(new BooleanExpression(codeRef,cCast(ExpressionTypes::Boolean),true));
                    else if( tmp == FALSE )
                        tokens.push_back(new BooleanExpression(codeRef,cCast(ExpressionTypes::Boolean),false));
                    else if( tmp == S_NULL )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::Null)));
                    else
                        tokens.push_back(new NameExpression(codeRef,cCast(ExpressionTypes::Name),tmp));
                }
                else if( sourceCode[i] >= '0' && sourceCode[i] <= '9' )
                {
                    size_t codeRef = i;
                    double tmp = sourceCode[i] - '0';
                    i++;
                    while( sourceCode[i] >= '0' && sourceCode[i] <= '9' )
                    {
                        tmp = tmp * 10 + (sourceCode[i] - '0');
                        i++;
                    }
                    if( sourceCode[i] == '.' )
                    {
                        i++;
                        if( sourceCode[i] >= '0' && sourceCode[i] <= '9' )
                        {
                            size_t cont=1;
                            while( sourceCode[i] >= '0' && sourceCode[i] <= '9' )
                            {
                                cont *= 10;
                                tmp += (double)(sourceCode[i] - '0') / cont;
                                i++;
                            }
                        }
                        else
                            throw SyntaxException("Expected decimal part of number",i);
                    }

                    if( (sourceCode[i] >= 'A' && sourceCode[i] <= 'Z') ||
                        (sourceCode[i] >= 'a' && sourceCode[i] <= 'z') ||
                        (sourceCode[i] == '_') ||
                        (sourceCode[i] == '.')
                        )
                        throw SyntaxException("Character not recognized as part of the number",i);

                    tokens.push_back(new NumericExpression(codeRef,cCast(ExpressionTypes::Numeric),tmp));
                }
                else
                    throw SyntaxException("Unrecognized character",i);
                i--;
            }
                break;
        }
        i++;
    }
    if( containersStack.empty() )
        return tokens;
    if( containersStack.top() == OPEN_PARENTHESIS )
        throw SyntaxException("Expected ')'",i);
    if( containersStack.top() == OPEN_BRACE )
        throw SyntaxException("Expected '}'",i);
    else
        throw SyntaxException("Expected ']'",i);
}
