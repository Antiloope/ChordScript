#include "lexer.h"
#include "utils/Exceptions/exception.h"
#include "utils/log.h"

#include <stack>

using namespace std;

const char cEqual               = '=';
const char cSubstraction        = '-';
const char cAddition            = '+';
const char cMultiplication      = '*';
const char cDivition            = '/';
const char cNegation            = '!';
const char cOr                  = '|';
const char cAnd                 = '&';
const char cMemberAccess        = '.';
const char cOpenParenthesis     = '(';
const char cCloseParenthesis    = ')';
const char cOpenBracket         = '[';
const char cCloseBracket        = ']';
const char cOpenBrace           = '{';
const char cCloseBrace          = '}';
const char cEndOfExpression     = ';';
const char cSeparator           = ',';
const char cStringDelimitator   = '"';
const char cStartComment        = '#';
const char cEndOfLine           = 0x0A;
const char cSpace               = ' ';
const string sForStatement      = "for";
const string sIfStatement       = "if";
const string sBreakStatement    = "break";
const string sReturnStatement   = "return";
const string sTrueStatement     = "true";
const string sFalseStatement    = "false";

namespace CS {

queue<Expression*> tokenize(string sourceCode){
    queue<Expression*> tokens;

    // To check correct number and order of parenthesis, brackets, and braces
    stack<char> containersStack;

    size_t i = 0;
    while ( i < sourceCode.length() ){

        switch(sourceCode[i]){
            case cEqual:
                tokens.push(new EqualExpression());
                i++;
                break;
            case cEndOfExpression:
                tokens.push(new EOEExpression());
                i++;
                break;
            case cSubstraction:
                tokens.push(new SubstractionExpression());
                i++;
                break;
            case cAddition:
                tokens.push(new AdditionExpression());
                i++;
                break;
            case cMultiplication:
                tokens.push(new MultiplicationExpression());
                i++;
                break;
            case cDivition:
                tokens.push(new DivitionExpression());
                i++;
                break;
            case cNegation:
                tokens.push(new NegationExpression());
                i++;
                break;
            case cOr:
                tokens.push(new OrExpression());
                i++;
                break;
            case cAnd:
                tokens.push(new AndExpression());
                i++;
                break;
            case cMemberAccess:
                tokens.push(new MemberAccessExpression());
                i++;
                break;
            case cSeparator:
                tokens.push(new SeparatorExpression());
                i++;
                break;
            case cOpenParenthesis:
                containersStack.push('(');
                tokens.push(new OpenParenthesisExpression());
                i++;
                break;
            case cCloseParenthesis:
                containersStack.top() == '(' ? containersStack.pop() : throw SyntaxException("Closing a wrong parenthesis", i);
                tokens.push(new CloseParenthesisExpression());
                i++;
                break;
            case cOpenBracket:
                containersStack.push('[');
                tokens.push(new OpenBracketExpression());
                i++;
                break;
            case cCloseBracket:
                containersStack.top() == '[' ? containersStack.pop() : throw SyntaxException("Closing a wrong bracket", i);
                tokens.push(new CloseBracketExpression());
                i++;
                break;
            case cOpenBrace:
                containersStack.push('{');
                tokens.push(new OpenBraceExpression());
                i++;
                break;
            case cCloseBrace:
                containersStack.top() == '{' ? containersStack.pop() : throw SyntaxException("Closing a wrong brace", i);
                tokens.push(new CloseBraceExpression());
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
                i++;
                while( sourceCode[i] != cStringDelimitator ){
                    if ( i >= sourceCode.length() ) throw SyntaxException("String delimitator never closed",i);
                    tmp.push_back(sourceCode[i]);
                    i++;
                }
                i++;
                tokens.push(new StringExpression(tmp));
                }
                break;
            default:{
                if( (sourceCode[i] >= 'A' && sourceCode[i] <= 'Z') ||
                    (sourceCode[i] >= 'a' && sourceCode[i] <= 'z') ||
                    (sourceCode[i] == '_')
                    ){
                    string tmp = "";
                    tmp.push_back(sourceCode[i]);
                    i++;
                    while(
                        (sourceCode[i] >= 'A' && sourceCode[i] <= 'Z') ||
                        (sourceCode[i] >= 'a' && sourceCode[i] <= 'z') ||
                        (sourceCode[i] == '_') ||
                        (sourceCode[i] > '0' && sourceCode[i] < '9')
                        ){
                        tmp.push_back(sourceCode[i]);
                        i++;
                    }
                    // Check for reseved keywords
                    if (tmp == sForStatement) tokens.push(new ForExpression());
                    else if (tmp == sIfStatement) tokens.push(new IfExpression());
                    else if (tmp == sBreakStatement) tokens.push(new BreakExpression());
                    else if (tmp == sReturnStatement) tokens.push(new ReturnExpression());
                    else if (tmp == sTrueStatement) tokens.push(new BooleanValueExpression(true));
                    else if (tmp == sFalseStatement) tokens.push(new BooleanValueExpression(false));
                    else tokens.push(new NameExpression(tmp));
                }
                else if (sourceCode[i] > '0' && sourceCode[i] < '9'){
                    bool isInteger = false;
                    double tmp = sourceCode[i] - '0';
                    i++;
                    while(sourceCode[i] > '0' && sourceCode[i] < '9'){
                        tmp = tmp*10 + (sourceCode[i] - '0');
                        i++;
                    }
                    if(sourceCode[i] == '.'){
                        i++;
                        if(sourceCode[i] > '0' && sourceCode[i] < '9'){
                            size_t cont=1;
                            while(sourceCode[i] > '0' && sourceCode[i] < '9'){
                                cont*=10;
                                tmp += (double)(sourceCode[i] - '0') / cont;
                                i++;
                            }
                        }else{
                            throw SyntaxException("Expected decimal part of number",i);
                        }
                    }else{
                        isInteger = true;
                    }

                    if( (sourceCode[i] >= 'A' && sourceCode[i] <= 'Z') ||
                        (sourceCode[i] >= 'a' && sourceCode[i] <= 'z') ||
                        (sourceCode[i] == '_') ||
                        (sourceCode[i] == '.')
                        ){
                        throw SyntaxException("Character not recognized as part of the number",i);
                    }

                    if(isInteger){
                        tokens.push(new IntegerNumberExpression((int)tmp));
                    }else{
                        tokens.push(new RealNumberExpression(tmp));
                    }
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

}
