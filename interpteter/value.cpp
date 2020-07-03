#include "value.h"
#include <stack>
#include "context.h"

using namespace CS;
using namespace std;

////////////////////////////////////////
///     Value
////////////////////////////////////////

Value::Value(string dataType) : _dataType(dataType) {}

Value::~Value() {}

string Value::getDataTypeName() const {
    return _dataType;
}

////////////////////////////////////////
///     LiteralValue : Value
////////////////////////////////////////

LiteralValue::LiteralValue(string dataType) : Value(dataType) {}

////////////////////////////////////////
///     LinkedValue : Value
////////////////////////////////////////

LinkedValue::LinkedValue() : Value("null") {}

////////////////////////////////////////
///     String
////////////////////////////////////////

string StringLiteralValue::getValue() const {
    return _text;
}

StringLiteralValue::StringLiteralValue(string text) : LiteralValue("string") {
    _text = text;
}

StringLinkedValue::StringLinkedValue(const StringExpression* stringExpression) {
    _text = stringExpression->getText();
}

LiteralValue* StringLinkedValue::getValue() const {
    return new StringLiteralValue(_text);
}

////////////////////////////////////////
///     Null
////////////////////////////////////////

NullLiteralValue::NullLiteralValue() : LiteralValue("null") {}

LiteralValue* NullLinkedValue::getValue() const {
    return new NullLiteralValue();
}

NullLinkedValue::NullLinkedValue() {}

////////////////////////////////////////
///     Numeric
////////////////////////////////////////

NumericLiteralValue::NumericLiteralValue(double value) : LiteralValue("numeric"), _value(value) {}

double NumericLiteralValue::getValue() const {
    return _value;
}

NumericLinkedValue::NumericLinkedValue(double value) : LinkedValue(), _value(value) {}

LiteralValue* NumericLinkedValue::getValue() const {
    return new NumericLiteralValue(_value);
}

////////////////////////////////////////
///     MathOperation
////////////////////////////////////////

MathOperationLinkedValue::MathOperationLinkedValue(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    auto hashCode = typeid (*tmp).hash_code();

    stack<char> RPNStack;

    //Check if first element of operation is a plus or minus sign to consider it or not in the operation
    if( hashCode == typeid (AdditionExpression).hash_code() ) {
        terminalExpressionsList->pop_front();
    }
    else if( hashCode == typeid (SubstractionExpression).hash_code() )
    {
        _linkedValuesList.push_back(new NumericLinkedValue(-1));
        RPNStack.push('*');
        terminalExpressionsList->pop_front();
    }
    tmp = terminalExpressionsList->front();
    hashCode = typeid (*tmp).hash_code();

    bool isValidExpression = true;
    while(isValidExpression)
    {
        // Check each kind of valid expression and store using RPN in _linkedValuesList.
        if( hashCode == typeid (OpenParenthesisExpression).hash_code() )
        {
            RPNStack.push('(');
        }
        else if ( hashCode == typeid (AdditionExpression).hash_code() )
        {
            while(
                !RPNStack.empty() &&
                (RPNStack.top() == '/' ||
                 RPNStack.top() == '-' ||
                 RPNStack.top() == '+' ||
                 RPNStack.top() == '*')
                ){
                _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
                RPNStack.pop();
            }
            RPNStack.push('+');
        }
        else if ( hashCode == typeid (SubstractionExpression).hash_code() )
        {
            while(
                !RPNStack.empty() &&
                (RPNStack.top() == '/' ||
                 RPNStack.top() == '-' ||
                 RPNStack.top() == '+' ||
                 RPNStack.top() == '*')
                ){
                _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
                RPNStack.pop();
            }
            RPNStack.push('-');
        }
        else if ( hashCode == typeid (MultiplicationExpression).hash_code() )
        {
            while(
                !RPNStack.empty() &&
                (RPNStack.top() == '/' ||
                 RPNStack.top() == '*')
                ){
                _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
                RPNStack.pop();
            }
            RPNStack.push('*');
        }
        else if ( hashCode == typeid (DivitionExpression).hash_code() )
        {
            while(
                !RPNStack.empty() &&
                (RPNStack.top() == '/' ||
                 RPNStack.top() == '*')
            ){
                _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
                RPNStack.pop();
            }
            RPNStack.push('/');
        }
        else if ( hashCode == typeid (NumericValueExpression).hash_code() )
        {
            _linkedValuesList.push_back(new NumericLinkedValue(((NumericValueExpression*)tmp)->getValue()));
        }
        else if ( hashCode == typeid (CloseParenthesisExpression).hash_code() )
        {
            while( !RPNStack.empty() && RPNStack.top() != '(' )
            {
                _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
                RPNStack.pop();
            }
            if ( !RPNStack.empty() && RPNStack.top() == '(' )
            {
                RPNStack.pop();
            }
            if ( RPNStack.empty() ) isValidExpression = false;
        }
        else if ( hashCode == typeid (NameExpression).hash_code() )
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if(it == terminalExpressionsList->end()) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            if (
                typeid (*tmp).hash_code() == typeid (MemberAccessExpression).hash_code() ||
                typeid (*tmp).hash_code() == typeid (OpenParenthesisExpression).hash_code()
                )
            {
                _linkedValuesList.push_back(new ExecutionLinkedValue(terminalExpressionsList));
            }
            else
            {
                tmp = terminalExpressionsList->front();
                if ( Context::getInstance()->nameExist(((NameExpression*)tmp)->getName()) )
                {
                    _linkedValuesList.push_back(new NameLinkedValue((NameExpression*)tmp));
                }
                else
                {
                    throw SemanticException("Unrecognized name", tmp->getCodeReference());
                }
            }
        }
        else // There are an invalid expression
        {
            isValidExpression = false;
        }
        if(isValidExpression)
        {
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();
            hashCode = typeid (*tmp).hash_code();
        }
    }
    while(!RPNStack.empty()) // Insert all operands in the list
    {
        if (
            RPNStack.top() == '*' ||
            RPNStack.top() == '-' ||
            RPNStack.top() == '+' ||
            RPNStack.top() == '/'
            )
        {
            _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
            RPNStack.pop();
        }
        else if (RPNStack.top() == '(')
        {
            throw SyntaxException("Expected )",tmp->getCodeReference());
        }
        else
        {
            throw SyntaxException("Invalid symbol at mathematical operation",tmp->getCodeReference());
        }
    }
}

// TODO: Implement RPN reading
LiteralValue* MathOperationLinkedValue::getValue() const {
    return nullptr;
}

////////////////////////////////////////
///     Operator
////////////////////////////////////////

OperatorLiteralValue::OperatorLiteralValue(char op) : LiteralValue("numeric"), _operator(op) {}

double OperatorLiteralValue::getValue(double a, double b) const {
    switch (_operator) {
    case '*':
        return a * b;
    case '/':
        return a / b;
    case '+':
        return a + b;
    case '-':
        return a - b;
    default:
        throw; //TODO: Create a new type of exception
        break;
    }
}

OperatorLinkedValue::OperatorLinkedValue(char op) : _operator(op) {}

LiteralValue * OperatorLinkedValue::getValue() const {
    return new OperatorLiteralValue(_operator);
}

////////////////////////////////////////
///     BooleanOperator
////////////////////////////////////////

BooleanOperationLinkedValue::BooleanOperationLinkedValue(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    auto hashCode = typeid (*tmp).hash_code();

    stack<char> RPNStack;

    bool isValidExpression = true;
    while(isValidExpression)
    {
        // Check each kind of valid expression and store using RPN in _linkedValuesList.
        if( hashCode == typeid (OpenParenthesisExpression).hash_code() )
        {
            RPNStack.push('(');
        }
        else if ( hashCode == typeid (EqualExpression).hash_code() )
        {
            if ( !RPNStack.empty() && RPNStack.top() != '(' )
            {
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());
            }
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();
            hashCode = typeid (*tmp).hash_code();
            if ( typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code() )
            {
                RPNStack.push('=');
            }
            else
            {
                throw SyntaxException("Expected symbol ==",tmp->getCodeReference());
            }
        }
        else if ( hashCode == typeid (NegationExpression).hash_code() )
        {
            if ( !RPNStack.empty() && RPNStack.top() != '(' )
            {
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());
            }
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();
            hashCode = typeid (*tmp).hash_code();
            if ( typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code() )
            {
                RPNStack.push('!');
            }
            else
            {
                throw SyntaxException("Expected symbol !=",tmp->getCodeReference());
            }
        }
        else if ( hashCode == typeid (GreaterThanExpression).hash_code() )
        {
            if ( !RPNStack.empty() && RPNStack.top() != '(' )
            {
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());
            }
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if(it == terminalExpressionsList->end()) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            if ( typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code() )
            {
                terminalExpressionsList->pop_front();
                RPNStack.push('g');
            }
            else
            {
                RPNStack.push('>');
            }
        }
        else if ( hashCode == typeid (LessThanExpression).hash_code() )
        {
            if ( !RPNStack.empty() && RPNStack.top() != '(' )
            {
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());
            }
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if(it == terminalExpressionsList->end()) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            if ( typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code() )
            {
                terminalExpressionsList->pop_front();
                RPNStack.push('l');
            }
            else
            {
                RPNStack.push('<');
            }
        }
        else if ( hashCode == typeid (StringExpression).hash_code() )
        {
            _linkedValuesList.push_back(new StringLinkedValue(((StringExpression*)tmp)));
        }
        else if ( hashCode == typeid (BooleanValueExpression).hash_code() )
        {
            _linkedValuesList.push_back(new BooleanLinkedValue(((BooleanValueExpression*)tmp)->getBoolValue()));
            terminalExpressionsList->push_front(nullptr);
        }
        else if(
            hashCode == typeid (AdditionExpression).hash_code() ||
            hashCode == typeid (SubstractionExpression).hash_code() ||
            hashCode == typeid (NumericValueExpression).hash_code() ||
            hashCode == typeid (NameExpression).hash_code())
        {
            _linkedValuesList.push_back(new MathOperationLinkedValue(terminalExpressionsList));
            terminalExpressionsList->push_front(nullptr);
        }
        else if ( hashCode == typeid (CloseParenthesisExpression).hash_code() )
        {
            while( !RPNStack.empty() && RPNStack.top() != '(' )
            {
                _linkedValuesList.push_back(new BooleanOperatorLinkedValue(RPNStack.top()));
                RPNStack.pop();
            }
            if ( !RPNStack.empty() && RPNStack.top() == '(')
            {
                RPNStack.pop();
            }
            if ( RPNStack.empty() ) isValidExpression = false;
        }
        else // There are an invalid expression
        {
            isValidExpression = false;
        }
        if(isValidExpression)
        {
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();
            hashCode = typeid (*tmp).hash_code();
        }
    }
    while(!RPNStack.empty()) // Insert all operands in the list
    {
        if (
            RPNStack.top() == '=' ||
            RPNStack.top() == '!' ||
            RPNStack.top() == '>' ||
            RPNStack.top() == 'g' ||
            RPNStack.top() == 'l' ||
            RPNStack.top() == '<'
            )
        {
            _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
            RPNStack.pop();
        }
        else if (RPNStack.top() == '(')
        {
            throw SyntaxException("Expected )",tmp->getCodeReference());
        }
        else
        {
            throw SyntaxException("Invalid symbol at mathematical operation",tmp->getCodeReference());
        }
    }
}

// TODO: Implement similar to MathOperation
LiteralValue* BooleanOperationLinkedValue::getValue() const {
    return nullptr;
}

////////////////////////////////////////
///     Array
////////////////////////////////////////

ArrayLinkedValue::ArrayLinkedValue(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    auto hashCode = typeid (*tmp).hash_code();

    if( hashCode == typeid (OpenParenthesisExpression).hash_code() )
    {
        _type = "argument";
    }
    else if ( hashCode == typeid (OpenBracketExpression).hash_code() )
    {
        _type = "array";
    }
    else
    {
        throw SyntaxException("Expected ( or [",tmp->getCodeReference());
    }
    bool isValidValue = true;
    while(isValidValue){
        terminalExpressionsList->pop_front();
        tmp = terminalExpressionsList->front();
        hashCode = typeid (*tmp).hash_code();

        if(hashCode == typeid (StringExpression).hash_code()){
            _linkedValuesList.push_back(new StringLinkedValue((StringExpression*)tmp));
        }
        else if(hashCode == typeid (NullValueExpression).hash_code())
        {
            _linkedValuesList.push_back(new NullLinkedValue());
        }
        else if(
            hashCode == typeid (AdditionExpression).hash_code() ||
            hashCode == typeid (SubstractionExpression).hash_code())
        {
            _linkedValuesList.push_back(new MathOperationLinkedValue(terminalExpressionsList));
        }
        else if(
            hashCode == typeid (NegationExpression).hash_code() ||
            hashCode == typeid (BooleanValueExpression).hash_code())
        {
            _linkedValuesList.push_back(new BooleanOperationLinkedValue(terminalExpressionsList));
        }
        else if(
            hashCode == typeid (OpenBracketExpression).hash_code() ||
            hashCode == typeid (OpenParenthesisExpression).hash_code())
        {
            _linkedValuesList.push_back(new ArrayLinkedValue(terminalExpressionsList));
        }
        else if(hashCode == typeid (NameExpression).hash_code())
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if(it == terminalExpressionsList->end()) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            hashCode = typeid (*tmp).hash_code();
            if(
                hashCode == typeid (OpenParenthesisExpression).hash_code() ||
                hashCode == typeid (MemberAccessExpression).hash_code())
            {
                _linkedValuesList.push_back(new ExecutionLinkedValue(terminalExpressionsList));
            }
            else if(
                hashCode == typeid (AdditionExpression).hash_code() ||
                hashCode == typeid (SubstractionExpression).hash_code() ||
                hashCode == typeid (DivitionExpression).hash_code() ||
                hashCode == typeid (MultiplicationExpression).hash_code()
                )
            {
                _linkedValuesList.push_back(new MathOperationLinkedValue(terminalExpressionsList));
            }
            else if(
                hashCode == typeid (EqualExpression).hash_code() ||
                hashCode == typeid (GreaterThanExpression).hash_code() ||
                hashCode == typeid (LessThanExpression).hash_code() ||
                hashCode == typeid (NegationExpression).hash_code()
                )
            {
                _linkedValuesList.push_back(new BooleanOperationLinkedValue(terminalExpressionsList));
            }
            else
            {
                _linkedValuesList.push_back(new NameLinkedValue((NameExpression*)terminalExpressionsList->front()));
            }
        }
        else if(hashCode == typeid (NumericValueExpression).hash_code())
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if(it == terminalExpressionsList->end()) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            if(
                typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code() ||
                typeid (*tmp).hash_code() == typeid (GreaterThanExpression).hash_code() ||
                typeid (*tmp).hash_code() == typeid (LessThanExpression).hash_code() ||
                typeid (*tmp).hash_code() == typeid (NegationExpression).hash_code()
                )
            {
                _linkedValuesList.push_back(new BooleanOperationLinkedValue(terminalExpressionsList));
            }
            else
            {
                _linkedValuesList.push_back(new MathOperationLinkedValue(terminalExpressionsList));
            }
        }
        else if ( hashCode == typeid (SeparatorExpression).hash_code() )
        {
        }
        else if (
            hashCode == typeid (CloseParenthesisExpression).hash_code() ||
            hashCode == typeid (CloseBracketExpression).hash_code())
        {
            terminalExpressionsList->pop_front();
            isValidValue = false;
        }
        else
        {
            throw SyntaxException("Expected a valid value",tmp->getCodeReference());
        }
    }
}

// TODO: Return argument or array type
LiteralValue* ArrayLinkedValue::getValue() const {
    return nullptr;
}

////////////////////////////////////////
///     Execution
////////////////////////////////////////

// TODO: Implment to store with a list of arguments and check access methods
ExecutionLinkedValue::ExecutionLinkedValue(list<TerminalExpression*>*) {

}

// TODO: Return numeric value result
LiteralValue* ExecutionLinkedValue::getValue() const {
    return nullptr;
}

////////////////////////////////////////
///     Name
////////////////////////////////////////

NameLinkedValue::NameLinkedValue(NameExpression* nameExpression){
    _name = nameExpression->getName();
    if ( !Context::getInstance()->isValidName(_name) ) throw SyntaxException("Invalid name",nameExpression->getCodeReference());
    if ( !Context::getInstance()->nameExist(_name) ) throw SemanticException("Unrecognized name",nameExpression->getCodeReference());
}

// TODO: Implement return the value finding in context
LiteralValue* NameLinkedValue::getValue() const {
    return nullptr;
}

////////////////////////////////////////
///     BooleanValue
////////////////////////////////////////

BooleanLiteralValue::BooleanLiteralValue(bool value) : LiteralValue("boolean"),_value(value){

}

bool BooleanLiteralValue::getValue() const {
    return _value;
}

BooleanLinkedValue::BooleanLinkedValue(bool value) : _value(value){

}

LiteralValue * BooleanLinkedValue::getValue() const {
    return new BooleanLiteralValue(_value);
}

////////////////////////////////////////
///     BooleanOperator
////////////////////////////////////////

BooleanOperatorLiteralValue::BooleanOperatorLiteralValue(char op) : LiteralValue("boolean"),_operator(op){
}

bool BooleanOperatorLiteralValue::getValue(bool a,bool b) const{
    switch (_operator) {
    case '=':
        return a == b;
    case '!':
        return a != b;
    case '<':
        return a < b;
    case '>':
        return a > b;
    case 'g':
        return a >= b;
    case 'l':
        return a <= b;
    default:
        throw; //TODO: Create a new type of exception
        break;
    }
}

bool BooleanOperatorLiteralValue::getValue(double a,double b) const{
    switch (_operator) {
    case '=':
        return a == b;
    case '!':
        return a != b;
    case '<':
        return a < b;
    case '>':
        return a > b;
    case 'g':
        return a >= b;
    case 'l':
        return a <= b;
    default:
        throw; //TODO: Create a new type of exception
        break;
    }
}

bool BooleanOperatorLiteralValue::getValue(string a,string b) const{
    switch (_operator) {
    case '=':
        return a == b;
    case '!':
        return a != b;
    case '<':
        return a < b;
    case '>':
        return a > b;
    case 'g':
        return a >= b;
    case 'l':
        return a <= b;
    default:
        throw; //TODO: Create a new type of exception
        break;
    }
}

BooleanOperatorLinkedValue::BooleanOperatorLinkedValue(char op) : _operator(op){

}

LiteralValue * BooleanOperatorLinkedValue::getValue() const {
    return new BooleanOperatorLiteralValue(_operator);
}

