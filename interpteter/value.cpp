#include "value.h"
#include <stack>

using namespace CS;
using namespace std;

Value::Value(string dataType) : _dataType(dataType) {}

Value::~Value() {}

string Value::getDataTypeName() const {
    return _dataType;
}

LiteralValue::LiteralValue(string dataType) : Value(dataType) {}

LinkedValue::LinkedValue() : Value("null") {}

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

LiteralValue* NullLinkedValue::getValue() const {
    return new NullLiteralValue();
}

NullLiteralValue::NullLiteralValue() : LiteralValue("null") {}

NullLinkedValue::NullLinkedValue() {}

NumericLiteralValue::NumericLiteralValue(double value) : LiteralValue("numeric"), _value(value) {}

double NumericLiteralValue::getValue() const {
    return _value;
}

MathOperationLinkedValue::MathOperationLinkedValue(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    auto hashCode = typeid (*tmp).hash_code();

    stack<char> RPNStack;

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
            while( RPNStack.top() != '(' )
            {
                if ( RPNStack.empty() ) throw SyntaxException("Closing a parenthesis not opened",tmp->getCodeReference());
                _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
                RPNStack.pop();
            }
            if (RPNStack.top() == '(')
            {
                RPNStack.pop();
            }
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
                _linkedValuesList.push_back(new NameLinkedValue((NameExpression*)tmp));
            }
        }
        else
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
    while(!RPNStack.empty())
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

LiteralValue* MathOperationLinkedValue::getValue() const {
    return nullptr;
}

OperatorLinkedValue::OperatorLinkedValue(char op) : _operator(op) {}

LiteralValue * OperatorLinkedValue::getValue() const {
    return new OperatorLiteralValue(_operator);
}

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

BooleanOperationLinkedValue::BooleanOperationLinkedValue(list<TerminalExpression*>*) {

}

LiteralValue* BooleanOperationLinkedValue::getValue() const {
    return nullptr;
}

ArrayLinkedValue::ArrayLinkedValue(list<TerminalExpression*>*) {

}

LiteralValue* ArrayLinkedValue::getValue() const {
    return nullptr;
}

ExecutionLinkedValue::ExecutionLinkedValue(list<TerminalExpression*>*) {

}

LiteralValue* ExecutionLinkedValue::getValue() const {
    return nullptr;
}

NameLinkedValue::NameLinkedValue(NameExpression*){

}

LiteralValue* NameLinkedValue::getValue() const {
    return nullptr;
}

NumericLinkedValue::NumericLinkedValue(double value) : LinkedValue(), _value(value) {}

LiteralValue* NumericLinkedValue::getValue() const {
    return new NumericLiteralValue(_value);
}
