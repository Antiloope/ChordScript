#include "lexer.h"
#include "utils/Exceptions/exception.h"
#include "utils/log.h"
#include "languageConstants.h"
#include "expressions/terminalexpression.h"
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
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::Equal)));
                break;
            case END_OF_EXPRESSION:
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::EOE)));
                break;
            case SUBSTRACTION:
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::Substract)));
                break;
            case ADDITION:
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::Addition)));
                break;
            case MULTIPLICATION:
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::Multiplication)));
                break;
            case DIVITION:
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::Divition)));
                break;
            case NEGATION:
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::Negation)));
                break;
            case GREATER_THAN:
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::GreaterThan)));
                break;
            case LESS_THAN:
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::LessThan)));
                break;
            case OR:
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::Or)));
                break;
            case AND:
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::And)));
                break;
            case MEMBER_ACCESS:
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::MemberAccess)));
                break;
            case SEPARATOR:
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::Separator)));
                break;
            case OPEN_PARENTHESIS:
                containersStack.push('(');
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::OpenParenthesis)));
                break;
            case CLOSE_PARENTHESIS:
                containersStack.top() == '('
                    ? containersStack.pop()
                    : throw SyntaxException("Closing a wrong parenthesis", i);

                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::CloseParenthesis)));
                break;
            case OPEN_BRACKET:
                containersStack.push('[');
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::OpenBracket)));
                break;
            case CLOSE_BRACKET:
                containersStack.top() == '['
                    ? containersStack.pop()
                    : throw SyntaxException("Closing a wrong bracket", i);

                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::CloseBracket)));
                break;
            case OPEN_BRACE:
                containersStack.push('{');
                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::OpenBrace)));
                break;
            case CLOSE_BRACE:
                containersStack.top() == '{'
                    ? containersStack.pop()
                    : throw SyntaxException("Closing a wrong brace", i);

                tokens.push_back(new TerminalExpression(i,cCast(TerminalExpressionType::CloseBrace)));
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
                tokens.push_back(new StringExpression(codeRef,cCast(TerminalExpressionType::String),tmp));
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
                        tokens.push_back(new TerminalExpression(codeRef,cCast(TerminalExpressionType::For)));
                    else if( tmp == IF )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(TerminalExpressionType::If)));
                    else if( tmp == ELSE )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(TerminalExpressionType::Else)));
                    else if( tmp == BREAK )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(TerminalExpressionType::Break)));
                    else if( tmp == RETURN )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(TerminalExpressionType::Return)));
                    else if( tmp == TRUE )
                        tokens.push_back(new BooleanExpression(codeRef,cCast(TerminalExpressionType::Boolean),true));
                    else if( tmp == FALSE )
                        tokens.push_back(new BooleanExpression(codeRef,cCast(TerminalExpressionType::Boolean),false));
                    else if( tmp == S_NULL )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(TerminalExpressionType::Null)));
                    else
                        tokens.push_back(new NameExpression(codeRef,cCast(TerminalExpressionType::Name),tmp));
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

                    tokens.push_back(new NumericExpression(codeRef,cCast(TerminalExpressionType::Numeric),tmp));
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
