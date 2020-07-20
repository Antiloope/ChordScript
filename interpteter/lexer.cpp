#include "lexer.h"
#include "utils/Exceptions/exception.h"
#include "utils/log.h"

#include <stack>
#include "languageConstants.h"

using namespace std;
using namespace CS;
using namespace CS::constants;

list<TerminalExpression*> Lexer::tokenize(string sourceCode){
    list<TerminalExpression*> tokens;

    // To check correct number and order of parenthesis, brackets, and braces
    stack<char> containersStack;

    size_t i = 0;
    while ( i < sourceCode.length() ){
        switch(sourceCode[i]){
            case cEqual:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Equal)));
                i++;
                break;
            case cEndOfExpression:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::EOE)));
                i++;
                break;
            case cSubstraction:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Substract)));
                i++;
                break;
            case cAddition:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Addition)));
                i++;
                break;
            case cMultiplication:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Multiplication)));
                i++;
                break;
            case cDivition:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Divition)));
                i++;
                break;
            case cNegation:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Negation)));
                i++;
                break;
            case cGreaterThan:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::GreaterThan)));
                i++;
                break;
            case cLessThan:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::LessThan)));
                i++;
                break;
            case cOr:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Or)));
                i++;
                break;
            case cAnd:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::And)));
                i++;
                break;
            case cMemberAccess:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::MemberAccess)));
                i++;
                break;
            case cSeparator:
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::Separator)));
                i++;
                break;
            case cOpenParenthesis:
                containersStack.push('(');
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::OpenParenthesis)));
                i++;
                break;
            case cCloseParenthesis:
                containersStack.top() == '(' ? containersStack.pop() : throw SyntaxException("Closing a wrong parenthesis", i);
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::CloseParenthesis)));
                i++;
                break;
            case cOpenBracket:
                containersStack.push('[');
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::OpenBracket)));
                i++;
                break;
            case cCloseBracket:
                containersStack.top() == '[' ? containersStack.pop() : throw SyntaxException("Closing a wrong bracket", i);
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::CloseBracket)));
                i++;
                break;
            case cOpenBrace:
                containersStack.push('{');
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::OpenBrace)));
                i++;
                break;
            case cCloseBrace:
                containersStack.top() == '{' ? containersStack.pop() : throw SyntaxException("Closing a wrong brace", i);
                tokens.push_back(new TerminalExpression(i,cCast(ExpressionTypes::CloseBrace)));
                i++;
                break;
            case cEndOfLine:
            case cSpace:
                i++;
                break;
            case cStartComment:
                i++;
                while( sourceCode[i] != cEndOfLine ){
                    if ( i >= sourceCode.length() ) break;
                    i++;
                }
                break;
            case cStringDelimitator: {
                string tmp = "";
                size_t codeRef = i;
                i++;
                while( sourceCode[i] != cStringDelimitator ){
                    if ( i >= sourceCode.length() ) throw SyntaxException("String delimitator never closed",i);
                    tmp.push_back(sourceCode[i]);
                    i++;
                }
                i++;
                tokens.push_back(new StringExpression(codeRef,cCast(ExpressionTypes::String),tmp));
                }
                break;
            default:{
                if( (sourceCode[i] >= 'A' && sourceCode[i] <= 'Z') ||
                    (sourceCode[i] >= 'a' && sourceCode[i] <= 'z') ||
                    (sourceCode[i] == '_')
                    ){
                    string tmp = "";
                    size_t codeRef = i;
                    tmp.push_back(sourceCode[i]);
                    i++;
                    while(
                        (sourceCode[i] >= 'A' && sourceCode[i] <= 'Z') ||
                        (sourceCode[i] >= 'a' && sourceCode[i] <= 'z') ||
                        (sourceCode[i] == '_') ||
                        (sourceCode[i] >= '0' && sourceCode[i] <= '9')
                        ){
                        tmp.push_back(sourceCode[i]);
                        i++;
                    }
                    // Check for reseved keywords
                    if (tmp == sFor) tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::For)));
                    else if (tmp == sIf) tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::If)));
                    else if (tmp == sElse) tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::Else)));
                    else if (tmp == sBreak) tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::Break)));
                    else if (tmp == sReturn) tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::Return)));
                    else if (tmp == sTrue) tokens.push_back(new BooleanExpression(codeRef,cCast(ExpressionTypes::Boolean),true));
                    else if (tmp == sFalse) tokens.push_back(new BooleanExpression(codeRef,cCast(ExpressionTypes::Boolean),false));
                    else if (tmp == sNull) tokens.push_back(new TerminalExpression(codeRef,cCast(ExpressionTypes::Null)));
                    else tokens.push_back(new NameExpression(codeRef,cCast(ExpressionTypes::Name),tmp));
                }
                else if (sourceCode[i] >= '0' && sourceCode[i] <= '9'){
                    size_t codeRef = i;
                    double tmp = sourceCode[i] - '0';
                    i++;
                    while(sourceCode[i] >= '0' && sourceCode[i] <= '9'){
                        tmp = tmp*10 + (sourceCode[i] - '0');
                        i++;
                    }
                    if(sourceCode[i] == '.'){
                        i++;
                        if(sourceCode[i] >= '0' && sourceCode[i] <= '9'){
                            size_t cont=1;
                            while(sourceCode[i] >= '0' && sourceCode[i] <= '9'){
                                cont*=10;
                                tmp += (double)(sourceCode[i] - '0') / cont;
                                i++;
                            }
                        }else{
                            throw SyntaxException("Expected decimal part of number",i);
                        }
                    }

                    if( (sourceCode[i] >= 'A' && sourceCode[i] <= 'Z') ||
                        (sourceCode[i] >= 'a' && sourceCode[i] <= 'z') ||
                        (sourceCode[i] == '_') ||
                        (sourceCode[i] == '.')
                        ){
                        throw SyntaxException("Character not recognized as part of the number",i);
                    }

                    tokens.push_back(new NumericExpression(codeRef,cCast(ExpressionTypes::Numeric),tmp));
                }else{
                    throw SyntaxException("Unrecognized character",i);
                }
                }
                break;
        }
    }
    if(containersStack.empty())
        return tokens;
    if(containersStack.top() == cOpenParenthesis)
        throw SyntaxException("Expected ')'",i);
    if(containersStack.top() == cOpenBrace)
        throw SyntaxException("Expected '}'",i);
    else
        throw SyntaxException("Expected ']'",i);
}
