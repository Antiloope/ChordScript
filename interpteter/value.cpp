#include "value.h"
#include <stack>
#include "context.h"

using namespace CS;
using namespace std;

////////////////////////////////////////
///     Value
////////////////////////////////////////

Value::Value(DataTypesId dataType) : _dataType(dataType) {}

Value::~Value() {}

DataTypesId Value::getDataTypeId() const {
    return _dataType;
}

////////////////////////////////////////
///     LiteralValue : Value
////////////////////////////////////////

LiteralValue::LiteralValue(DataTypesId dataType) : Value(dataType) {}

LiteralValue::LiteralValue(const LiteralValue& e) : Value(e._dataType),_value(e._value) {}

LiteralValue::~LiteralValue() {}

StringLiteralValue::StringLiteralValue(string text) : LiteralValue(DataTypesId::String) {
    _text = text;
    _value = &_text;
}

StringLiteralValue::~StringLiteralValue() {}

NumericLiteralValue::NumericLiteralValue(double number) : LiteralValue(DataTypesId::Numeric) {
    _number = number;
    _value = &_number;
}

NumericLiteralValue::~NumericLiteralValue() {}

BooleanLiteralValue::BooleanLiteralValue(bool boolean) : LiteralValue(DataTypesId::Boolean) {
    _boolean = boolean;
    _value = &_boolean;
}

BooleanLiteralValue::~BooleanLiteralValue() {}

NullLiteralValue::NullLiteralValue() : LiteralValue(DataTypesId::Null) {
    _value = nullptr;
}

NullLiteralValue::~NullLiteralValue() {}

OperatorLiteralValue::OperatorLiteralValue(char op) : LiteralValue(DataTypesId::Operator) {
    _operator = op;
    _value = &_operator;
}

OperatorLiteralValue::~OperatorLiteralValue() {}

////////////////////////////////////////
///     LinkedValue : Value
////////////////////////////////////////

LinkedValue::LinkedValue(size_t codeReference) : Value(DataTypesId::Null), _codeReference(codeReference) {}

LinkedValue::~LinkedValue() {}

LinkedValue* LinkedValue::generateLinkedValue(list<TerminalExpression*>* terminalExpressionsList){
    if( terminalExpressionsList->empty() ) throw SyntaxException("Expected a value");

    TerminalExpression* tmp = terminalExpressionsList->front();

    LinkedValue* ret;

    switch ( tmp->getType() )
    {
    case cCast(ExpressionTypes::String):
        ret = new StringLinkedValue(tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::Null):
        ret = new NullLinkedValue(tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::Addition):
    case cCast(ExpressionTypes::Substract):
        ret = new MathOperationLinkedValue(tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::OpenBracket):
    case cCast(ExpressionTypes::OpenParenthesis):
        ret = new ArrayLinkedValue(tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::Boolean):
    case cCast(ExpressionTypes::Negation):
        ret = new BooleanOperationLinkedValue(tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::Name):
    {
        auto it = terminalExpressionsList->begin();
        advance(it,1);
        if( it == terminalExpressionsList->end() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
        tmp = *it;
        switch (tmp->getType()) {
        case cCast(ExpressionTypes::OpenParenthesis):
        case cCast(ExpressionTypes::MemberAccess):
            ret = new ExecutionLinkedValue(tmp->getCodeReference());
            break;
        case cCast(ExpressionTypes::Addition):
        case cCast(ExpressionTypes::Substract):
        case cCast(ExpressionTypes::Divition):
        case cCast(ExpressionTypes::Multiplication):
            ret = new MathOperationLinkedValue(tmp->getCodeReference());
            break;
        case cCast(ExpressionTypes::Equal):
        case cCast(ExpressionTypes::GreaterThan):
        case cCast(ExpressionTypes::LessThan):
        case cCast(ExpressionTypes::Negation):
            ret = new BooleanOperationLinkedValue(tmp->getCodeReference());
            break;
        default:
            ret = new NameLinkedValue(tmp->getCodeReference());
            break;
        }
        break;
    }
    case cCast(ExpressionTypes::Numeric):
    {
        auto it = terminalExpressionsList->begin();
        advance(it,1);
        if(it == terminalExpressionsList->end()) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
        tmp = *it;
        if(
            tmp->getType() == cCast(ExpressionTypes::Equal) ||
            tmp->getType() == cCast(ExpressionTypes::GreaterThan) ||
            tmp->getType() == cCast(ExpressionTypes::LessThan)||
            tmp->getType() == cCast(ExpressionTypes::Negation)
            )
        {
            ret = new BooleanOperationLinkedValue(tmp->getCodeReference());
        }
        else
        {
            ret = new MathOperationLinkedValue(tmp->getCodeReference());
        }
        break;
    }
    default:
        throw SyntaxException("Expected a valid value",tmp->getCodeReference());
        break;
    }
    ret->load(terminalExpressionsList);
    return ret;
}

size_t LinkedValue::getCodeReference() const {
    return _codeReference;
}

////////////////////////////////////////
///     String
////////////////////////////////////////

StringLinkedValue::StringLinkedValue(size_t codeReference) : LinkedValue(codeReference) {}

void StringLinkedValue::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();
    if( tmp->getType() != cCast(ExpressionTypes::String) ) throw SyntaxException("Expected a string",tmp->getCodeReference());

    _text = ((StringExpression*)tmp)->getText();

    terminalExpressionsList->pop_front();
}

LiteralValue* StringLinkedValue::getValue() const {
    return new StringLiteralValue(_text);
}

////////////////////////////////////////
///     Null
////////////////////////////////////////

NullLinkedValue::NullLinkedValue(size_t codeReference) : LinkedValue(codeReference) {}

LiteralValue* NullLinkedValue::getValue() const {
    return new NullLiteralValue();
}

void NullLinkedValue::load(list<TerminalExpression*>* terminalExpressionsList) {
    if( terminalExpressionsList->front()->getType() != cCast(ExpressionTypes::Null) ) throw SyntaxException("Expected null", terminalExpressionsList->front()->getCodeReference());
    terminalExpressionsList->pop_front();
}

////////////////////////////////////////
///     Numeric
////////////////////////////////////////

NumericLinkedValue::NumericLinkedValue(size_t codeReference) : LinkedValue(codeReference) {}

void NumericLinkedValue::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();
    if( tmp->getType() != cCast(ExpressionTypes::Numeric) ) throw SyntaxException("Expected a numeric value",tmp->getCodeReference());

    _value = ((NumericExpression*)tmp)->getValue();

    terminalExpressionsList->pop_front();
}

LiteralValue* NumericLinkedValue::getValue() const {
    return new NumericLiteralValue(_value);
}

////////////////////////////////////////
///     MathOperation
////////////////////////////////////////

MathOperationLinkedValue::MathOperationLinkedValue(size_t codeReference) : LinkedValue(codeReference) {}

void MathOperationLinkedValue::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    stack<char> RPNStack;

    //Check if first element of operation is a plus or minus sign to consider it or not in the operation
    if ( tmp->getType() == cCast(ExpressionTypes::Addition) )
    {
        terminalExpressionsList->pop_front();
    }
    else if ( tmp->getType() == cCast(ExpressionTypes::Substract) )
    {
        _linkedValuesList.push_back(new NumericLinkedValue(tmp->getCodeReference()));
        TerminalExpression* minus = new NumericExpression(tmp->getCodeReference(),cCast(ExpressionTypes::Numeric),-1);
        terminalExpressionsList->push_front(minus);
        _linkedValuesList.back()->load(terminalExpressionsList);
        delete minus;
        RPNStack.push(cCast(MathSymbols::Multiplication));
    }
    tmp = terminalExpressionsList->front();

    bool isValidExpression = true;
    while(isValidExpression)
    {
        switch (tmp->getType()) {
        case cCast(ExpressionTypes::OpenParenthesis):
            RPNStack.push(cCast(MathSymbols::OpenParenthesis));
            break;
        case cCast(ExpressionTypes::Addition):
            while(
                !RPNStack.empty() &&
                (RPNStack.top() == cCast(MathSymbols::Divition) ||
                 RPNStack.top() == cCast(MathSymbols::Substraction) ||
                 RPNStack.top() == cCast(MathSymbols::Addition) ||
                 RPNStack.top() == cCast(MathSymbols::Multiplication)) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            RPNStack.push(cCast(MathSymbols::Addition));
            break;
        case cCast(ExpressionTypes::Substract):
            while(
                !RPNStack.empty() &&
                (RPNStack.top() == cCast(MathSymbols::Divition) ||
                 RPNStack.top() == cCast(MathSymbols::Substraction) ||
                 RPNStack.top() == cCast(MathSymbols::Addition) ||
                 RPNStack.top() == cCast(MathSymbols::Multiplication))
                ){
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            RPNStack.push(cCast(MathSymbols::Substraction));
            break;
        case cCast(ExpressionTypes::Multiplication):
            while(
                !RPNStack.empty() &&
                (RPNStack.top() == cCast(MathSymbols::Divition) ||
                 RPNStack.top() == cCast(MathSymbols::Multiplication))
                ){
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            RPNStack.push(cCast(MathSymbols::Multiplication));
            break;
        case cCast(ExpressionTypes::Divition):
            while(
                !RPNStack.empty() &&
                (RPNStack.top() == cCast(MathSymbols::Divition) ||
                 RPNStack.top() == cCast(MathSymbols::Multiplication))
                ){
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            RPNStack.push(cCast(MathSymbols::Divition));
            break;
        case cCast(ExpressionTypes::Numeric):
            _linkedValuesList.push_back(new NumericLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            terminalExpressionsList->push_front(nullptr);
            break;
        case cCast(ExpressionTypes::CloseParenthesis):
            while( !RPNStack.empty() && RPNStack.top() != cCast(MathSymbols::OpenParenthesis) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            if ( !RPNStack.empty() && RPNStack.top() == cCast(MathSymbols::OpenParenthesis) )
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
                _linkedValuesList.push_back(new ExecutionLinkedValue(tmp->getCodeReference()));
                _linkedValuesList.back()->load(terminalExpressionsList);
                terminalExpressionsList->push_front(nullptr);
            }
            else
            {
                tmp = terminalExpressionsList->front();
                if ( Context::getInstance()->nameExist(((NameExpression*)tmp)->getName()) )
                {
                    _linkedValuesList.push_back(new NameLinkedValue(tmp->getCodeReference()));
                    _linkedValuesList.back()->load(terminalExpressionsList);
                    terminalExpressionsList->push_front(nullptr);
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
            RPNStack.top() == cCast(MathSymbols::Multiplication) ||
            RPNStack.top() == cCast(MathSymbols::Substraction) ||
            RPNStack.top() == cCast(MathSymbols::Addition) ||
            RPNStack.top() == cCast(MathSymbols::Divition) )
        {
            auto op = new OperatorLinkedValue(tmp->getCodeReference());
            _linkedValuesList.push_back(op);
            op->load(RPNStack.top());
            RPNStack.pop();
        }
        else if ( RPNStack.top() == cCast(MathSymbols::OpenParenthesis) )
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
    stack<double> RPNStack;
    for( LinkedValue* linkedValue : _linkedValuesList )
    {
        LiteralValue* literalValue = linkedValue->getValue();

        switch (literalValue->getDataTypeId()) {
        case DataTypesId::Numeric:
            RPNStack.push(*(double*)literalValue->getValue());
            break;
        case DataTypesId::Operator:
        {
            if( RPNStack.empty() )
            {
                delete literalValue;
                throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
            }
            double op1 = RPNStack.top();
            RPNStack.pop();
            if( RPNStack.empty() )
            {
                delete literalValue;
                throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
            }

            double op2 = RPNStack.top();
            RPNStack.pop();
            switch (*(char*)literalValue->getValue()) {
            case cCast(MathSymbols::Multiplication):
                RPNStack.push(op2 * op1);
                break;
            case cCast(MathSymbols::Divition):
                if( op1 == 0 )
                {
                    delete literalValue;
                    throw SemanticException("Dividing by zero",linkedValue->getCodeReference());
                }
                RPNStack.push(op2 / op1);
                break;
            case cCast(MathSymbols::Addition):
                RPNStack.push(op2 + op1);
                break;
            case cCast(MathSymbols::Substraction):
                RPNStack.push(op2 - op1);
                break;
            default:
                delete literalValue;
                throw SyntaxException("Unknown operator",linkedValue->getCodeReference());
                break;
            }
        }
            break;
        default:
            string errorDescription = "Invalid convertion from ";
            errorDescription.append(DataType::getDataTypeString(literalValue->getDataTypeId()));
            errorDescription.append(" to numeric");
            delete literalValue;
            throw SemanticException(errorDescription,linkedValue->getCodeReference());
            break;
        }
        delete literalValue;
    }
    if ( RPNStack.empty() ) throw SyntaxException("Invalid number of operands",this->getCodeReference());
    double ret = RPNStack.top();
    RPNStack.pop();
    if ( !RPNStack.empty() ) throw SyntaxException("Invalid number of operands",this->getCodeReference());
    return new NumericLiteralValue(ret);
}

////////////////////////////////////////
///     Operator
////////////////////////////////////////

OperatorLinkedValue::OperatorLinkedValue(size_t codeReference) : LinkedValue(codeReference) {}

void OperatorLinkedValue::load(list<TerminalExpression*>* terminalExpressionsList) {
    // NOT IMPLEMENTED
    terminalExpressionsList->pop_front();
}

void OperatorLinkedValue::load(char op) {
    _operator = op;
}

LiteralValue * OperatorLinkedValue::getValue() const {
    return new OperatorLiteralValue(_operator);
}

////////////////////////////////////////
///     BooleanOperator
////////////////////////////////////////

BooleanOperationLinkedValue::BooleanOperationLinkedValue(size_t codeReference) : LinkedValue(codeReference) {}

void BooleanOperationLinkedValue::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    stack<char> RPNStack;

    bool isValidExpression = true;
    while(isValidExpression)
    {
        // Check each kind of valid expression and store using RPN in _linkedValuesList.
        switch ( tmp->getType() )
        {
        case cCast(ExpressionTypes::OpenParenthesis):
            RPNStack.push(cCast(BooleanSymbols::OpenParenthesis));
            break;
        case cCast(ExpressionTypes::And):
            while(
                !RPNStack.empty() &&
                RPNStack.top() != cCast(BooleanSymbols::OpenParenthesis) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            RPNStack.push(cCast(BooleanSymbols::And));
            break;
        case cCast(ExpressionTypes::Or):
            while(
                !RPNStack.empty() &&
                RPNStack.top() != cCast(BooleanSymbols::OpenParenthesis) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            RPNStack.push(cCast(BooleanSymbols::Or));
            break;
        case cCast(ExpressionTypes::Equal):
            if ( !RPNStack.empty() && RPNStack.top() != cCast(BooleanSymbols::OpenParenthesis) && RPNStack.top() != cCast(BooleanSymbols::And) && RPNStack.top() != cCast(BooleanSymbols::Or) && RPNStack.top() != cCast(BooleanSymbols::Negation) )
            {
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());
            }
            if ( !RPNStack.empty() && RPNStack.top() == cCast(BooleanSymbols::Negation) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();
            if ( tmp->getType() == cCast(ExpressionTypes::Equal) )
            {
                RPNStack.push(cCast(BooleanSymbols::Equal));
            }
            else
            {
                throw SyntaxException("Expected symbol ==",tmp->getCodeReference());
            }
            break;
        case cCast(ExpressionTypes::Negation):
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if( it == terminalExpressionsList->end() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            if ( tmp->getType() == cCast(ExpressionTypes::Equal) )
            {
                if ( !RPNStack.empty() && RPNStack.top() != cCast(BooleanSymbols::OpenParenthesis) && RPNStack.top() != cCast(BooleanSymbols::And) && RPNStack.top() != cCast(BooleanSymbols::Or) && RPNStack.top() != cCast(BooleanSymbols::Negation) )
                {
                    throw SyntaxException("Invalid bool operation",tmp->getCodeReference());
                }
                if ( !RPNStack.empty() && RPNStack.top() == cCast(BooleanSymbols::Negation) )
                {
                    auto op = new OperatorLinkedValue(tmp->getCodeReference());
                    _linkedValuesList.push_back(op);
                    op->load(RPNStack.top());
                    RPNStack.pop();
                }
                terminalExpressionsList->pop_front();
                RPNStack.push(cCast(BooleanSymbols::NotEqual));
            }
            else
            {
                RPNStack.push(cCast(BooleanSymbols::Negation));
            }
            tmp = terminalExpressionsList->front();
        }
            break;
        case cCast(ExpressionTypes::GreaterThan):
        {
            if ( !RPNStack.empty() && RPNStack.top() != cCast(BooleanSymbols::OpenParenthesis) && RPNStack.top() != cCast(BooleanSymbols::And) && RPNStack.top() != cCast(BooleanSymbols::Or) && RPNStack.top() != cCast(BooleanSymbols::Negation) )
            {
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());
            }
            if ( !RPNStack.empty() && RPNStack.top() == cCast(BooleanSymbols::Negation) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if( it == terminalExpressionsList->end() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            if( tmp->getType() == cCast(ExpressionTypes::Equal) )
            {
                terminalExpressionsList->pop_front();
                RPNStack.push(cCast(BooleanSymbols::GreaterEqual));
            }
            else
            {
                RPNStack.push(cCast(BooleanSymbols::Greater));
            }
        }
            break;
        case cCast(ExpressionTypes::LessThan):
        {
            if ( !RPNStack.empty() && RPNStack.top() != cCast(BooleanSymbols::OpenParenthesis) && RPNStack.top() != cCast(BooleanSymbols::And) && RPNStack.top() != cCast(BooleanSymbols::Or) && RPNStack.top() != cCast(BooleanSymbols::Negation) )
            {
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());
            }
            if ( !RPNStack.empty() && RPNStack.top() == cCast(BooleanSymbols::Negation) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if( it == terminalExpressionsList->end() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            tmp = *it;
            if( tmp->getType() == cCast(ExpressionTypes::Equal) )
            {
                terminalExpressionsList->pop_front();
                RPNStack.push(cCast(BooleanSymbols::LessEqual));
            }
            else
            {
                RPNStack.push(cCast(BooleanSymbols::Less));
            }
        }
            break;
        case cCast(ExpressionTypes::String):
            _linkedValuesList.push_back(new StringLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            terminalExpressionsList->push_front(nullptr);
            break;
        case cCast(ExpressionTypes::Boolean):
            _linkedValuesList.push_back(new BooleanLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            terminalExpressionsList->push_front(nullptr);
            break;
        case cCast(ExpressionTypes::Null):
            _linkedValuesList.push_back(new NullLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            terminalExpressionsList->push_front(nullptr);
            break;
        case cCast(ExpressionTypes::Addition):
        case cCast(ExpressionTypes::Substract):
        case cCast(ExpressionTypes::Numeric):
        case cCast(ExpressionTypes::Name):
            _linkedValuesList.push_back(new MathOperationLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            terminalExpressionsList->push_front(nullptr);
            break;
        case cCast(ExpressionTypes::CloseParenthesis):
            while( !RPNStack.empty() && RPNStack.top() != cCast(BooleanSymbols::OpenParenthesis) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            if ( !RPNStack.empty() && RPNStack.top() == cCast(BooleanSymbols::OpenParenthesis))
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
            RPNStack.top() == cCast(BooleanSymbols::Equal) ||
            RPNStack.top() == cCast(BooleanSymbols::Negation) ||
            RPNStack.top() == cCast(BooleanSymbols::Greater) ||
            RPNStack.top() == cCast(BooleanSymbols::GreaterEqual) ||
            RPNStack.top() == cCast(BooleanSymbols::LessEqual) ||
            RPNStack.top() == cCast(BooleanSymbols::Less) ||
            RPNStack.top() == cCast(BooleanSymbols::And) ||
            RPNStack.top() == cCast(BooleanSymbols::NotEqual) ||
            RPNStack.top() == cCast(BooleanSymbols::Or) )
        {
            auto op = new OperatorLinkedValue(tmp->getCodeReference());
            _linkedValuesList.push_back(op);
            op->load(RPNStack.top());
            RPNStack.pop();
        }
        else if ( RPNStack.top() == cCast(BooleanSymbols::OpenParenthesis) )
        {
            throw SyntaxException("Expected )",tmp->getCodeReference());
        }
        else
        {
            throw SyntaxException("Invalid symbol at mathematical operation",tmp->getCodeReference());
        }
    }
}

BooleanOperationLinkedValue::~BooleanOperationLinkedValue() {

}

LiteralValue* BooleanOperationLinkedValue::getValue() const {
    stack<LiteralValue*> RPNStack;
    for( LinkedValue* linkedValue : _linkedValuesList )
    {
        LiteralValue* literalValue = linkedValue->getValue();

        switch (literalValue->getDataTypeId()) {
        case DataTypesId::Numeric:
        case DataTypesId::String:
        case DataTypesId::Null:
        case DataTypesId::Boolean:
            RPNStack.push(literalValue);
            break;
        case DataTypesId::Operator:
        {
            if( RPNStack.empty() )
            {
                delete literalValue;
                throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
            }
            if( *(char*)literalValue->getValue() == cCast(BooleanSymbols::Negation) )
            {
                if( RPNStack.top()->getDataTypeId() == DataTypesId::Boolean )
                {
                    bool* tmp = (bool*)RPNStack.top()->getValue();
                    *tmp = !*tmp;
                }
                else if( RPNStack.top()->getDataTypeId() == DataTypesId::Numeric )
                {
                    double tmp = (*(double*)RPNStack.top()->getValue());
                    delete RPNStack.top();
                    RPNStack.pop();
                    RPNStack.push(new BooleanLiteralValue(tmp==0));
                }
                else if( RPNStack.top()->getDataTypeId() == DataTypesId::String )
                {
                    string tmp = (*(string*)RPNStack.top()->getValue());
                    delete RPNStack.top();
                    RPNStack.pop();
                    RPNStack.push(new BooleanLiteralValue(tmp.empty()));
                }
                else
                {
                    while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                    delete literalValue;
                    throw SemanticException("Invalid conversion to boolean",linkedValue->getCodeReference());
                }
            }
            else
            {
                switch ( RPNStack.top()->getDataTypeId() ) {
                case DataTypesId::Numeric:
                {
                    double op1, op2 = *(double*)RPNStack.top()->getValue();
                    bool isBoolean = false,bop1, bop2 = op2;
                    delete RPNStack.top();
                    RPNStack.pop();
                    if( RPNStack.empty() )
                    {
                        delete literalValue;
                        throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                    }
                    switch ( RPNStack.top()->getDataTypeId() ) {
                    case DataTypesId::Numeric:
                        op1 = *(double*)RPNStack.top()->getValue();
                        break;
                    case DataTypesId::Boolean:
                        isBoolean = true;
                        bop1 = *(bool*)RPNStack.top()->getValue();
                        break;
                    default:
                        while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                        delete literalValue;
                        throw SyntaxException("Invalid conversion",linkedValue->getCodeReference());
                        break;
                    }
                    delete RPNStack.top();
                    RPNStack.pop();
                    switch ( *(char*)literalValue->getValue() )
                    {
                    case cCast(BooleanSymbols::Or):
                        RPNStack.push(new BooleanLiteralValue(!isBoolean?op1||op2:bop1||bop2));
                        break;
                    case cCast(BooleanSymbols::And):
                        RPNStack.push(new BooleanLiteralValue(!isBoolean?op1&&op2:bop1&&bop2));
                        break;
                    case cCast(BooleanSymbols::Equal):
                        RPNStack.push(new BooleanLiteralValue(!isBoolean?op1==op2:bop1==bop2));
                        break;
                    case cCast(BooleanSymbols::NotEqual):
                        RPNStack.push(new BooleanLiteralValue(!isBoolean?op1!=op2:bop1!=bop2));
                        break;
                    case cCast(BooleanSymbols::Greater):
                        if( isBoolean )
                        {
                            while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                            delete literalValue;
                            throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                        }
                        RPNStack.push(new BooleanLiteralValue(op1>op2));
                        break;
                    case cCast(BooleanSymbols::GreaterEqual):
                        if( isBoolean )
                        {
                            while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                            delete literalValue;
                            throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                        }
                        RPNStack.push(new BooleanLiteralValue(op1>=op2));
                        break;
                    case cCast(BooleanSymbols::Less):
                        if( isBoolean )
                        {
                            while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                            delete literalValue;
                            throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                        }
                        RPNStack.push(new BooleanLiteralValue(op1<op2));
                        break;
                    case cCast(BooleanSymbols::LessEqual):
                        if( isBoolean )
                        {
                            while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                            delete literalValue;
                            throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                        }
                        RPNStack.push(new BooleanLiteralValue(op1<=op2));
                        break;
                    default:
                        while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                        delete literalValue;
                        throw SyntaxException("Unknown operator",linkedValue->getCodeReference());
                        break;
                    }
                }
                    break;
                case DataTypesId::String:
                {
                    string op1, op2 = *(string*)RPNStack.top()->getValue();
                    bool isBoolean = false,bop1, bop2 = !op2.empty();
                    delete RPNStack.top();
                    RPNStack.pop();
                    if( RPNStack.empty() )
                    {
                        delete literalValue;
                        throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                    }
                    switch ( RPNStack.top()->getDataTypeId() ) {
                    case DataTypesId::String:
                        op1 = *(string*)RPNStack.top()->getValue();
                        break;
                    case DataTypesId::Boolean:
                        isBoolean = true;
                        bop1 = *(bool*)RPNStack.top()->getValue();
                        break;
                    case DataTypesId::Null:
                        isBoolean = true;
                        bop1 = false;
                        break;
                    default:
                        while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                        delete literalValue;
                        throw SyntaxException("Invalid conversion",linkedValue->getCodeReference());
                        break;
                    }
                    delete RPNStack.top();
                    RPNStack.pop();
                    switch ( *(char*)literalValue->getValue() )
                    {
                    case cCast(BooleanSymbols::Or):
                        RPNStack.push(new BooleanLiteralValue(!isBoolean?op1.empty()||op2.empty():bop2||bop2));
                        break;
                    case cCast(BooleanSymbols::And):
                        RPNStack.push(new BooleanLiteralValue(!isBoolean?op1.empty()&&op2.empty():bop2&&bop2));
                        break;
                    case cCast(BooleanSymbols::Equal):
                        RPNStack.push(new BooleanLiteralValue(!isBoolean?op1==op2:bop1==bop2));
                        break;
                    case cCast(BooleanSymbols::NotEqual):
                        RPNStack.push(new BooleanLiteralValue(!isBoolean?op1!=op2:bop1!=bop2));
                        break;
                    default:
                        while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                        delete literalValue;
                        throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                        break;
                    }
                }
                    break;
                case DataTypesId::Null:
                {
                    bool bop1, bop2 = false;
                    delete RPNStack.top();
                    RPNStack.pop();
                    if( RPNStack.empty() )
                    {
                        delete literalValue;
                        throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                    }
                    switch ( RPNStack.top()->getDataTypeId() ) {
                    case DataTypesId::String:
                        bop1 = !(*(string*)RPNStack.top()->getValue()).empty();
                        break;
                    case DataTypesId::Boolean:
                        bop1 = *(bool*)RPNStack.top()->getValue();
                        break;
                    case DataTypesId::Null:
                        bop1 = false;
                        break;
                    case DataTypesId::Numeric:
                        bop1 = *(double*)RPNStack.top()->getValue();
                        break;
                    default:
                        while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                        delete literalValue;
                        throw SyntaxException("Invalid conversion",linkedValue->getCodeReference());
                        break;
                    }
                    delete RPNStack.top();
                    RPNStack.pop();
                    switch ( *(char*)literalValue->getValue() )
                    {
                    case cCast(BooleanSymbols::Or):
                        RPNStack.push(new BooleanLiteralValue(bop2||bop2));
                        break;
                    case cCast(BooleanSymbols::And):
                        RPNStack.push(new BooleanLiteralValue(bop2&&bop2));
                        break;
                    case cCast(BooleanSymbols::Equal):
                        RPNStack.push(new BooleanLiteralValue(bop1==bop2));
                        break;
                    case cCast(BooleanSymbols::NotEqual):
                        RPNStack.push(new BooleanLiteralValue(bop1!=bop2));
                        break;
                    default:
                        while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                        delete literalValue;
                        throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                        break;
                    }
                }
                    break;
                case DataTypesId::Boolean:
                {
                    bool bop1, bop2 = *(bool*)RPNStack.top()->getValue();
                    delete RPNStack.top();
                    RPNStack.pop();
                    if( RPNStack.empty() )
                    {
                        delete literalValue;
                        throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                    }
                    switch ( RPNStack.top()->getDataTypeId() ) {
                    case DataTypesId::String:
                        bop1 = !(*(string*)RPNStack.top()->getValue()).empty();
                        break;
                    case DataTypesId::Boolean:
                        bop1 = *(bool*)RPNStack.top()->getValue();
                        break;
                    case DataTypesId::Null:
                        bop1 = false;
                        break;
                    case DataTypesId::Numeric:
                        bop1 = *(double*)RPNStack.top()->getValue();
                        break;
                    default:
                        while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                        delete literalValue;
                        throw SyntaxException("Invalid conversion",linkedValue->getCodeReference());
                        break;
                    }
                    delete RPNStack.top();
                    RPNStack.pop();
                    switch ( *(char*)literalValue->getValue() )
                    {
                    case cCast(BooleanSymbols::Or):
                        RPNStack.push(new BooleanLiteralValue(bop2||bop2));
                        break;
                    case cCast(BooleanSymbols::And):
                        RPNStack.push(new BooleanLiteralValue(bop2&&bop2));
                        break;
                    case cCast(BooleanSymbols::Equal):
                        RPNStack.push(new BooleanLiteralValue(bop1==bop2));
                        break;
                    case cCast(BooleanSymbols::NotEqual):
                        RPNStack.push(new BooleanLiteralValue(bop1!=bop2));
                        break;
                    default:
                        while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                        delete literalValue;
                        throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                        break;
                    }
                }
                    break;
                default:
                    while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
                    delete literalValue;
                    throw SyntaxException("Unknown literal value",linkedValue->getCodeReference());
                    break;
                }
            }
        }
        break;
        default:
            string errorDescription = "Invalid convertion from ";
            errorDescription.append(DataType::getDataTypeString(literalValue->getDataTypeId()));
            errorDescription.append(" to boolean");
            while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
            delete literalValue;
            throw SemanticException(errorDescription,linkedValue->getCodeReference());
            break;
        }
    }
    if ( RPNStack.empty() ) throw SyntaxException("Invalid number of operands",this->getCodeReference());
    bool ret = *(bool*)RPNStack.top()->getValue();
    delete RPNStack.top();
    RPNStack.pop();
    if ( !RPNStack.empty() )
    {
        while(!RPNStack.empty()) {delete RPNStack.top();RPNStack.pop();}
        throw SyntaxException("Invalid number of operands",this->getCodeReference());
    }
    return new BooleanLiteralValue(ret);
}

////////////////////////////////////////
///     Array
////////////////////////////////////////

ArrayLinkedValue::ArrayLinkedValue(size_t codeReference) : LinkedValue(codeReference) {}

void ArrayLinkedValue::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    if( tmp->getType() == cCast(ExpressionTypes::OpenParenthesis) )
    {
        _type = DataTypesId::Argument;
    }
    else if ( tmp->getType() == cCast(ExpressionTypes::OpenBracket) )
    {
        _type = DataTypesId::Array;
    }
    else
    {
        throw SyntaxException("Expected ( or [",tmp->getCodeReference());
    }
    terminalExpressionsList->pop_front();

    bool isValidValue = true;
    while( isValidValue )
    {
        tmp = terminalExpressionsList->front();
        switch ( tmp->getType() )
        {
        case cCast(ExpressionTypes::String):
            _linkedValuesList.push_back(new StringLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            break;
        case cCast(ExpressionTypes::Null):
            _linkedValuesList.push_back(new NullLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            break;
        case cCast(ExpressionTypes::Addition):
        case cCast(ExpressionTypes::Substract):
            _linkedValuesList.push_back(new MathOperationLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            break;
        case cCast(ExpressionTypes::Negation):
        case cCast(ExpressionTypes::Boolean):
            _linkedValuesList.push_back(new BooleanOperationLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            break;
        case cCast(ExpressionTypes::OpenBracket):
        case cCast(ExpressionTypes::OpenParenthesis):
            _linkedValuesList.push_back(new ArrayLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
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
                _linkedValuesList.push_back(new ExecutionLinkedValue(tmp->getCodeReference()));
                _linkedValuesList.back()->load(terminalExpressionsList);
                break;
            case cCast(ExpressionTypes::Addition):
            case cCast(ExpressionTypes::Substract):
            case cCast(ExpressionTypes::Divition):
            case cCast(ExpressionTypes::Multiplication):
                _linkedValuesList.push_back(new MathOperationLinkedValue(tmp->getCodeReference()));
                _linkedValuesList.back()->load(terminalExpressionsList);
                break;
            case cCast(ExpressionTypes::Equal):
            case cCast(ExpressionTypes::GreaterThan):
            case cCast(ExpressionTypes::LessThan):
            case cCast(ExpressionTypes::Negation):
                _linkedValuesList.push_back(new BooleanOperationLinkedValue(tmp->getCodeReference()));
                _linkedValuesList.back()->load(terminalExpressionsList);
                break;
            default:
                _linkedValuesList.push_back(new NameLinkedValue(tmp->getCodeReference()));
                _linkedValuesList.back()->load(terminalExpressionsList);
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
                _linkedValuesList.push_back(new BooleanOperationLinkedValue(tmp->getCodeReference()));
                _linkedValuesList.back()->load(terminalExpressionsList);
            }
            else
            {
                _linkedValuesList.push_back(new MathOperationLinkedValue(tmp->getCodeReference()));
                _linkedValuesList.back()->load(terminalExpressionsList);
            }
        }
            break;
        case cCast(ExpressionTypes::Separator):
            terminalExpressionsList->pop_front();
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

ExecutionLinkedValue::ExecutionLinkedValue(size_t codeReference) : LinkedValue(codeReference) {}

void ExecutionLinkedValue::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    if ( tmp->getType() != cCast(ExpressionTypes::Name) ) throw SyntaxException("Expected a name", tmp->getCodeReference());

    _name = ((NameExpression*)tmp)->getName();
    if ( !Context::getInstance()->isValidName(_name) ) throw SyntaxException("Invalid name",tmp->getCodeReference());
    if ( !Context::getInstance()->functionNameExist(_name) ) throw SemanticException("Unrecognized name",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if ( terminalExpressionsList->empty() ) throw SyntaxException("Expected arguments",tmp->getCodeReference());
    tmp = terminalExpressionsList->front();

    if ( tmp->getType() == cCast(ExpressionTypes::OpenParenthesis) )
    {
        ArrayLinkedValue* aux = new ArrayLinkedValue(tmp->getCodeReference());
        _methodsList.push_back(tuple<string,ArrayLinkedValue*>(_name,aux));
        aux->load(terminalExpressionsList);
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

            ArrayLinkedValue* aux = new ArrayLinkedValue(tmp->getCodeReference());
            _methodsList.push_back(tuple<string,ArrayLinkedValue*>(name,aux));
            aux->load(terminalExpressionsList);
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

NameLinkedValue::NameLinkedValue(size_t codeReference) : LinkedValue(codeReference) {}

void NameLinkedValue::load(list<TerminalExpression*>* terminalExpressionsList){
    if ( terminalExpressionsList->empty() ) throw SyntaxException("Expected a name");
    NameExpression* nameExpression = (NameExpression*)terminalExpressionsList->front();
    _name = nameExpression->getName();
    if ( !Context::getInstance()->isValidName(_name) ) throw SyntaxException("Invalid name",nameExpression->getCodeReference());
    if ( !Context::getInstance()->nameExist(_name) ) throw SemanticException("Unrecognized name",nameExpression->getCodeReference());
    terminalExpressionsList->pop_front();
}

// TODO: Implement return the value finding in context
LiteralValue* NameLinkedValue::getValue() const {
    return nullptr;
}

////////////////////////////////////////
///     BooleanValue
////////////////////////////////////////

BooleanLinkedValue::BooleanLinkedValue(size_t codeReference) : LinkedValue(codeReference) {}

void BooleanLinkedValue::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();
    if( tmp->getType() != cCast(ExpressionTypes::Boolean) ) throw SyntaxException("Expected a boolean value",tmp->getCodeReference());

    _value= ((BooleanExpression*)tmp)->getValue();

    terminalExpressionsList->pop_front();
}

LiteralValue * BooleanLinkedValue::getValue() const {
    return new BooleanLiteralValue(_value);
}
