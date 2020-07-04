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

LiteralValue::LiteralValue(string dataType, void* value) : Value(dataType), _value(value) {}

// TODO: Implement deleting void* casting correctly
LiteralValue::~LiteralValue(){
}

////////////////////////////////////////
///     LinkedValue : Value
////////////////////////////////////////

LinkedValue::LinkedValue() : Value("null") {}

LinkedValue::~LinkedValue() {}

////////////////////////////////////////
///     String
////////////////////////////////////////

StringLinkedValue::StringLinkedValue(const StringExpression* stringExpression) {
    _text = stringExpression->getText();
}

LiteralValue* StringLinkedValue::getValue() const {
    return new LiteralValue("string",(void*)&_text);
}

////////////////////////////////////////
///     Null
////////////////////////////////////////

LiteralValue* NullLinkedValue::getValue() const {
    return new LiteralValue("null",nullptr);
}

NullLinkedValue::NullLinkedValue() {}

////////////////////////////////////////
///     Numeric
////////////////////////////////////////

NumericLinkedValue::NumericLinkedValue(double value) : LinkedValue(), _value(value) {}

LiteralValue* NumericLinkedValue::getValue() const {
    return new LiteralValue("numeric",(void*)&_value);
}

////////////////////////////////////////
///     MathOperation
////////////////////////////////////////

MathOperationLinkedValue::MathOperationLinkedValue(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    stack<char> RPNStack;

    //Check if first element of operation is a plus or minus sign to consider it or not in the operation
    if ( tmp->getType() == cCast(ExpressionTypes::Addition) )
    {
        terminalExpressionsList->pop_front();
    }
    else if ( tmp->getType() == cCast(ExpressionTypes::Substract) )
    {
        _linkedValuesList.push_back(new NumericLinkedValue(-1));
        RPNStack.push('*');
        terminalExpressionsList->pop_front();
    }
    tmp = terminalExpressionsList->front();

    bool isValidExpression = true;
    while(isValidExpression)
    {
        switch (tmp->getType()) {
        case cCast(ExpressionTypes::OpenParenthesis):
            RPNStack.push('(');
            break;
        case cCast(ExpressionTypes::Addition):
            while(
                !RPNStack.empty() &&
                (RPNStack.top() == '/' ||
                 RPNStack.top() == '-' ||
                 RPNStack.top() == '+' ||
                 RPNStack.top() == '*') )
            {
                _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
                RPNStack.pop();
            }
            RPNStack.push('+');
            break;
        case cCast(ExpressionTypes::Substract):
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
            break;
        case cCast(ExpressionTypes::Multiplication):
            while(
                !RPNStack.empty() &&
                (RPNStack.top() == '/' ||
                 RPNStack.top() == '*')
                ){
                _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
                RPNStack.pop();
            }
            RPNStack.push('*');
            break;
        case cCast(ExpressionTypes::Divition):
            while(
                !RPNStack.empty() &&
                (RPNStack.top() == '/' ||
                 RPNStack.top() == '*')
                ){
                _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
                RPNStack.pop();
            }
            RPNStack.push('/');
            break;
        case cCast(ExpressionTypes::Numeric):
            _linkedValuesList.push_back(new NumericLinkedValue(((NumericExpression*)tmp)->getValue()));
            break;
        case cCast(ExpressionTypes::CloseParenthesis):
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
            break;
        case cCast(ExpressionTypes::Name):
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if( it == terminalExpressionsList->end() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            if (
                tmp->getType() == cCast(ExpressionTypes::MemberAccess) ||
                tmp->getType() == cCast(ExpressionTypes::OpenParenthesis) )
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
            break;
        }
        default:
            isValidExpression = false;
            break;
        }
        if( isValidExpression )
        {
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();
        }
    }
    while( !RPNStack.empty() ) // Insert all operands in the list
    {
        if (
            RPNStack.top() == '*' ||
            RPNStack.top() == '-' ||
            RPNStack.top() == '+' ||
            RPNStack.top() == '/' )
        {
            _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
            RPNStack.pop();
        }
        else if ( RPNStack.top() == '(' )
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

OperatorLinkedValue::OperatorLinkedValue(char op) : _operator(op) {}

LiteralValue * OperatorLinkedValue::getValue() const {
    return new LiteralValue("operator",(void*)&_operator);
}

////////////////////////////////////////
///     BooleanOperator
////////////////////////////////////////

BooleanOperationLinkedValue::BooleanOperationLinkedValue(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    stack<char> RPNStack;

    bool isValidExpression = true;
    while(isValidExpression)
    {
        // Check each kind of valid expression and store using RPN in _linkedValuesList.
        switch ( tmp->getType() )
        {
        case cCast(ExpressionTypes::OpenParenthesis):
            RPNStack.push('(');
            break;
        case cCast(ExpressionTypes::Equal):
            if ( !RPNStack.empty() && RPNStack.top() != '(' )
            {
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());
            }
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();
            if ( tmp->getType() == cCast(ExpressionTypes::Equal) )
            {
                RPNStack.push('=');
            }
            else
            {
                throw SyntaxException("Expected symbol ==",tmp->getCodeReference());
            }
            break;
        case cCast(ExpressionTypes::Negation):
            if ( !RPNStack.empty() && RPNStack.top() != '(' )
            {
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());
            }
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();
            if ( tmp->getType() == cCast(ExpressionTypes::Equal) )
            {
                RPNStack.push('!');
            }
            else
            {
                throw SyntaxException("Expected symbol !=",tmp->getCodeReference());
            }
            break;
        case cCast(ExpressionTypes::GreaterThan):
        {
            if ( !RPNStack.empty() && RPNStack.top() != '(' )
            {
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());
            }
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if( it == terminalExpressionsList->end() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            if( tmp->getType() == cCast(ExpressionTypes::Equal) )
            {
                terminalExpressionsList->pop_front();
                RPNStack.push('g');
            }
            else
            {
                RPNStack.push('>');
            }
        }
            break;
        case cCast(ExpressionTypes::LessThan):
        {
            if ( !RPNStack.empty() && RPNStack.top() != '(' )
            {
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());
            }
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if( it == terminalExpressionsList->end() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            if( tmp->getType() == cCast(ExpressionTypes::Equal) )
            {
                terminalExpressionsList->pop_front();
                RPNStack.push('l');
            }
            else
            {
                RPNStack.push('<');
            }
        }
            break;
        case cCast(ExpressionTypes::String):
            _linkedValuesList.push_back(new StringLinkedValue(((StringExpression*)tmp)));
            break;
        case cCast(ExpressionTypes::Boolean):
            _linkedValuesList.push_back(new BooleanLinkedValue(((BooleanExpression*)tmp)->getValue()));
            terminalExpressionsList->push_front(nullptr);
            break;
        case cCast(ExpressionTypes::Addition):
        case cCast(ExpressionTypes::Substract):
        case cCast(ExpressionTypes::Numeric):
        case cCast(ExpressionTypes::Name):
            _linkedValuesList.push_back(new MathOperationLinkedValue(terminalExpressionsList));
            terminalExpressionsList->push_front(nullptr);
            break;
        case cCast(ExpressionTypes::CloseParenthesis):
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
            break;
        default:
            isValidExpression = false;
            break;
        }
        if( isValidExpression )
        {
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();
        }
    }
    while( !RPNStack.empty() ) // Insert all operands in the list
    {
        if (
            RPNStack.top() == '=' ||
            RPNStack.top() == '!' ||
            RPNStack.top() == '>' ||
            RPNStack.top() == 'g' ||
            RPNStack.top() == 'l' ||
            RPNStack.top() == '<' )
        {
            _linkedValuesList.push_back(new OperatorLinkedValue(RPNStack.top()));
            RPNStack.pop();
        }
        else if ( RPNStack.top() == '(' )
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

    if( tmp->getType() == cCast(ExpressionTypes::OpenParenthesis) )
    {
        _type = "argument";
    }
    else if ( tmp->getType() == cCast(ExpressionTypes::OpenBracket) )
    {
        _type = "array";
    }
    else
    {
        throw SyntaxException("Expected ( or [",tmp->getCodeReference());
    }
    terminalExpressionsList->pop_front();
    tmp = terminalExpressionsList->front();

    bool isValidValue = true;
    while( isValidValue )
    {
        switch ( tmp->getType() )
        {
        case cCast(ExpressionTypes::String):
            _linkedValuesList.push_back(new StringLinkedValue((StringExpression*)tmp));
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();
            break;
        case cCast(ExpressionTypes::Null):
            _linkedValuesList.push_back(new NullLinkedValue());
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();
            break;
        case cCast(ExpressionTypes::Addition):
        case cCast(ExpressionTypes::Substract):
            _linkedValuesList.push_back(new MathOperationLinkedValue(terminalExpressionsList));
            break;
        case cCast(ExpressionTypes::Negation):
        case cCast(ExpressionTypes::Boolean):
            _linkedValuesList.push_back(new BooleanOperationLinkedValue(terminalExpressionsList));
            break;
        case cCast(ExpressionTypes::OpenBracket):
        case cCast(ExpressionTypes::OpenParenthesis):
            _linkedValuesList.push_back(new ArrayLinkedValue(terminalExpressionsList));
            break;
        case cCast(ExpressionTypes::Name):
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if(it == terminalExpressionsList->end()) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            switch ( tmp->getType() )
            {
            case cCast(ExpressionTypes::OpenParenthesis):
            case cCast(ExpressionTypes::MemberAccess):
                _linkedValuesList.push_back(new ExecutionLinkedValue(terminalExpressionsList));
                break;
            case cCast(ExpressionTypes::Addition):
            case cCast(ExpressionTypes::Substract):
            case cCast(ExpressionTypes::Divition):
            case cCast(ExpressionTypes::Multiplication):
                _linkedValuesList.push_back(new MathOperationLinkedValue(terminalExpressionsList));
                break;
            case cCast(ExpressionTypes::Equal):
            case cCast(ExpressionTypes::GreaterThan):
            case cCast(ExpressionTypes::LessThan):
            case cCast(ExpressionTypes::Negation):
                _linkedValuesList.push_back(new BooleanOperationLinkedValue(terminalExpressionsList));
                break;
            default:
                _linkedValuesList.push_back(new NameLinkedValue((NameExpression*)terminalExpressionsList->front()));
                break;
            }
        }
            break;
        case cCast(ExpressionTypes::Numeric):
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if( it == terminalExpressionsList->end() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            if(
                tmp->getType() == cCast(ExpressionTypes::Equal) ||
                tmp->getType() == cCast(ExpressionTypes::GreaterThan) ||
                tmp->getType() == cCast(ExpressionTypes::LessThan) ||
                tmp->getType() == cCast(ExpressionTypes::Negation) )
            {
                _linkedValuesList.push_back(new BooleanOperationLinkedValue(terminalExpressionsList));
            }
            else
            {
                _linkedValuesList.push_back(new MathOperationLinkedValue(terminalExpressionsList));
            }
        }
            break;
        case cCast(ExpressionTypes::Separator):
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();
            break;
        case cCast(ExpressionTypes::CloseParenthesis):
        case cCast(ExpressionTypes::CloseBracket):
            terminalExpressionsList->pop_front();
            isValidValue = false;
            break;
        default:
            throw SyntaxException("Expected a valid value",tmp->getCodeReference());
            break;
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

ExecutionLinkedValue::ExecutionLinkedValue(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    if ( tmp->getType() != cCast(ExpressionTypes::Name) ) throw SyntaxException("Expected a name", tmp->getCodeReference());

    _name = ((NameExpression*)tmp)->getName();
    if ( !Context::getInstance()->isValidName(_name) ) throw SyntaxException("Invalid name",tmp->getCodeReference());
    if ( !Context::getInstance()->nameExist(_name) ) throw SemanticException("Unrecognized name",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if ( terminalExpressionsList->empty() ) throw SyntaxException("Expected arguments",tmp->getCodeReference());
    tmp = terminalExpressionsList->front();

    if ( tmp->getType() == cCast(ExpressionTypes::OpenParenthesis) )
    {
        _methodsList.push_back(tuple<string,ArrayLinkedValue*>(_name,new ArrayLinkedValue(terminalExpressionsList)));
    }
    bool isValidMethod = true;
    while(isValidMethod)
    {
        if( tmp->getType() == cCast(ExpressionTypes::MemberAccess) )
        {
            terminalExpressionsList->pop_front();
            if ( terminalExpressionsList->empty() ) throw SyntaxException("Expected arguments",tmp->getCodeReference());
            tmp = terminalExpressionsList->front();

            if ( tmp->getType() != cCast(ExpressionTypes::Name) ) throw SyntaxException("Expected a name", tmp->getCodeReference());

            string name = ((NameExpression*)tmp)->getName();

            terminalExpressionsList->pop_front();
            if ( terminalExpressionsList->empty() ) throw SyntaxException("Expected (",tmp->getCodeReference());
            tmp = terminalExpressionsList->front();

            if ( tmp->getType() != cCast(ExpressionTypes::OpenParenthesis) ) throw SyntaxException("Expected (",tmp->getCodeReference());

            _methodsList.push_back(tuple<string,ArrayLinkedValue*>(name,new ArrayLinkedValue(terminalExpressionsList)));
        }
        else
        {
            isValidMethod = false;
        }
    }
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

BooleanLinkedValue::BooleanLinkedValue(bool value) : _value(value){

}

LiteralValue * BooleanLinkedValue::getValue() const {
    return new LiteralValue("boolean",(void*)&_value);
}

////////////////////////////////////////
///     BooleanOperator
////////////////////////////////////////

BooleanOperatorLinkedValue::BooleanOperatorLinkedValue(char op) : _operator(op){

}

LiteralValue * BooleanOperatorLinkedValue::getValue() const {
    return new LiteralValue("operator",(void*)&_operator);
}

