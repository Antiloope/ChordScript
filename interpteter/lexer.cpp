#include "lexer.h"
#include "utils/Exceptions/exception.h"
#include "utils/log.h"
#include "languageConstants.h"
#include "terminalexpressions.h"
#include <stack>

using namespace std;
using namespace CS;
using namespace CS::constants;

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
            case cEqual:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Equal)));
                break;
            case cEndOfExpression:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::EOE)));
                break;
            case cSubstraction:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Substract)));
                break;
            case cAddition:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Addition)));
                break;
            case cMultiplication:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Multiplication)));
                break;
            case cDivition:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Divition)));
                break;
            case cNegation:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Negation)));
                break;
            case cGreaterThan:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::GreaterThan)));
                break;
            case cLessThan:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::LessThan)));
                break;
            case cOr:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Or)));
                break;
            case cAnd:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::And)));
                break;
            case cMemberAccess:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::MemberAccess)));
                break;
            case cSeparator:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Separator)));
                break;
            case cOpenParenthesis:
                containersStack.push('(');
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::OpenParenthesis)));
                break;
            case cCloseParenthesis:
                containersStack.top() == '('
                    ? containersStack.pop()
                    : throw SyntaxException("Closing a wrong parenthesis", i);

                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::CloseParenthesis)));
                break;
            case cOpenBracket:
                containersStack.push('[');
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::OpenBracket)));
                break;
            case cCloseBracket:
                containersStack.top() == '['
                    ? containersStack.pop()
                    : throw SyntaxException("Closing a wrong bracket", i);

                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::CloseBracket)));
                break;
            case cOpenBrace:
                containersStack.push('{');
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::OpenBrace)));
                break;
            case cCloseBrace:
                containersStack.top() == '{'
                    ? containersStack.pop()
                    : throw SyntaxException("Closing a wrong brace", i);

                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::CloseBrace)));
                break;
            case cEndOfLine:
            case cSpace:
                break;
            case cStartComment:
                i++;
                while( sourceCode[i] != cEndOfLine )
                {
                    if( i >= sourceCode.length() )
                        break;
                    i++;
                }
                i--;
                break;
            case cStringDelimitator:
            {
                string tmp = "";
                size_t codeRef = i;
                i++;
                while( sourceCode[i] != cStringDelimitator )
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
                    if( tmp == sFor )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::For)));
                    else if( tmp == sIf )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::If)));
                    else if( tmp == sElse )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::Else)));
                    else if( tmp == sBreak )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::Break)));
                    else if( tmp == sReturn )
                        tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::Return)));
                    else if( tmp == sTrue )
                        tokens.push_back(new BooleanExpression(codeRef,cCast(ExpressionTypes::Boolean),true));
                    else if( tmp == sFalse )
                        tokens.push_back(new BooleanExpression(codeRef,cCast(ExpressionTypes::Boolean),false));
                    else if( tmp == sNull )
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
    if( containersStack.top() == cOpenParenthesis )
        throw SyntaxException("Expected ')'",i);
    if( containersStack.top() == cOpenBrace )
        throw SyntaxException("Expected '}'",i);
    else
        throw SyntaxException("Expected ']'",i);
}
