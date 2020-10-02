#include "booleanoperationlinkedvalue.h"
#include "interpreter/expressions/terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/values/literalvalue.h"
#include "interpreter/players/soundgenerator.h"
#include "interpreter/players/sampleplayer.h"
#include <stack>
#include <memory>

using namespace CS;
using namespace std;

BooleanOperationLinkedValue::BooleanOperationLinkedValue(size_t codeReference) :
    LinkedValue(codeReference) {}

void BooleanOperationLinkedValue::load(
    list<TerminalExpression*>* terminalExpressionsList
    ) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    stack<char> RPNStack;

    bool isValidExpression = true;
    while( isValidExpression )
    {
        // Check each kind of valid expression and store using RPN in _linkedValuesList.
        switch ( tmp->getType() )
        {
        case cCast(TerminalExpressionType::OpenParenthesis):
            RPNStack.push(cCast(BooleanSymbols::OpenParenthesis));
            break;
        case cCast(TerminalExpressionType::And):
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
        case cCast(TerminalExpressionType::Or):
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
        case cCast(TerminalExpressionType::Equal):
            if ( !RPNStack.empty() && RPNStack.top() != cCast(BooleanSymbols::OpenParenthesis) && RPNStack.top() != cCast(BooleanSymbols::And) && RPNStack.top() != cCast(BooleanSymbols::Or) && RPNStack.top() != cCast(BooleanSymbols::Negation) )
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());

            if ( !RPNStack.empty() && RPNStack.top() == cCast(BooleanSymbols::Negation) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            terminalExpressionsList->pop_front();
            tmp = terminalExpressionsList->front();

            if ( tmp->getType() == cCast(TerminalExpressionType::Equal) )
                RPNStack.push(cCast(BooleanSymbols::Equal));
            else
                throw SyntaxException("Expected symbol ==",tmp->getCodeReference());

            break;
        case cCast(TerminalExpressionType::Negation):
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);

            if( it == terminalExpressionsList->end() )
                throw SyntaxException("Expected another symbol",tmp->getCodeReference());

            tmp = *it;

            if ( tmp->getType() == cCast(TerminalExpressionType::Equal) )
            {
                if (
                    !RPNStack.empty() && RPNStack.top() != cCast(BooleanSymbols::OpenParenthesis) &&
                    RPNStack.top() != cCast(BooleanSymbols::And) &&
                    RPNStack.top() != cCast(BooleanSymbols::Or) &&
                    RPNStack.top() != cCast(BooleanSymbols::Negation)
                    )
                    throw SyntaxException("Invalid bool operation",tmp->getCodeReference());

                if (
                    !RPNStack.empty() &&
                    RPNStack.top() == cCast(BooleanSymbols::Negation) )
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
        case cCast(TerminalExpressionType::GreaterThan):
        {
            if (
                !RPNStack.empty() &&
                RPNStack.top() != cCast(BooleanSymbols::OpenParenthesis) &&
                RPNStack.top() != cCast(BooleanSymbols::And) &&
                RPNStack.top() != cCast(BooleanSymbols::Or) &&
                RPNStack.top() != cCast(BooleanSymbols::Negation)
                )
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());

            if (
                !RPNStack.empty() &&
                RPNStack.top() == cCast(BooleanSymbols::Negation) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }

            auto it = terminalExpressionsList->begin();
            advance(it,1);

            if( it == terminalExpressionsList->end() )
                throw SyntaxException("Expected another symbol",tmp->getCodeReference());

            tmp = *it;

            if( tmp->getType() == cCast(TerminalExpressionType::Equal) )
            {
                terminalExpressionsList->pop_front();
                RPNStack.push(cCast(BooleanSymbols::GreaterEqual));
            }
            else
                RPNStack.push(cCast(BooleanSymbols::Greater));

        }
        break;
        case cCast(TerminalExpressionType::LessThan):
        {
            if (
                !RPNStack.empty() &&
                RPNStack.top() != cCast(BooleanSymbols::OpenParenthesis) &&
                RPNStack.top() != cCast(BooleanSymbols::And) &&
                RPNStack.top() != cCast(BooleanSymbols::Or) &&
                RPNStack.top() != cCast(BooleanSymbols::Negation)
                )
                throw SyntaxException("Invalid bool operation",tmp->getCodeReference());

            if (
                !RPNStack.empty() &&
                RPNStack.top() == cCast(BooleanSymbols::Negation) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }

            auto it = terminalExpressionsList->begin();
            advance(it,1);

            if( it == terminalExpressionsList->end() )
                throw SyntaxException("Expected another symbol",tmp->getCodeReference());

            tmp = *it;

            if( tmp->getType() == cCast(TerminalExpressionType::Equal) )
            {
                terminalExpressionsList->pop_front();
                RPNStack.push(cCast(BooleanSymbols::LessEqual));
            }
            else
                RPNStack.push(cCast(BooleanSymbols::Less));
        }
        break;
        case cCast(TerminalExpressionType::String):
            _linkedValuesList.push_back(new StringLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            terminalExpressionsList->push_front(nullptr);
            break;
        case cCast(TerminalExpressionType::Boolean):
            _linkedValuesList.push_back(new BooleanLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            terminalExpressionsList->push_front(nullptr);
            break;
        case cCast(TerminalExpressionType::Null):
            _linkedValuesList.push_back(new NullLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            terminalExpressionsList->push_front(nullptr);
            break;
        case cCast(TerminalExpressionType::Addition):
        case cCast(TerminalExpressionType::Substract):
        case cCast(TerminalExpressionType::Numeric):
        case cCast(TerminalExpressionType::Name):
            _linkedValuesList.push_back(new MathOperationLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            terminalExpressionsList->push_front(nullptr);
            break;
        case cCast(TerminalExpressionType::CloseParenthesis):
            while(
                !RPNStack.empty() &&
                RPNStack.top() != cCast(BooleanSymbols::OpenParenthesis) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }

            if (
                !RPNStack.empty() &&
                RPNStack.top() == cCast(BooleanSymbols::OpenParenthesis)
                )
                RPNStack.pop();

            if ( RPNStack.empty() )
                isValidExpression = false;

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
            throw SyntaxException("Expected )",tmp->getCodeReference());
        else
            throw SyntaxException("Invalid symbol at mathematical operation",tmp->getCodeReference());
    }
}

BooleanOperationLinkedValue::~BooleanOperationLinkedValue() {
    while( !_linkedValuesList.empty() )
    {
        if( _linkedValuesList.front() )
            delete _linkedValuesList.front();
        _linkedValuesList.pop_front();
    }
}

LiteralValue* BooleanOperationLinkedValue::getValue() const {
    stack<LiteralValue*> RPNStack;
    for( LinkedValue* linkedValue : _linkedValuesList )
    {
        unique_ptr<LiteralValue> literalValue = unique_ptr<LiteralValue>(linkedValue->getValue());

        switch( literalValue->getDataTypeId() )
        {
        case DataTypesId::Numeric:
        case DataTypesId::String:
        case DataTypesId::Null:
        case DataTypesId::Boolean:
            RPNStack.push(literalValue->clone());
            break;
        case DataTypesId::Operator:
        {
            if( RPNStack.empty() )
                throw SyntaxException("Invalid operation",linkedValue->getCodeReference());

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
                    while(!RPNStack.empty())
                    {
                        delete RPNStack.top();
                        RPNStack.pop();
                    }
                    throw SemanticException("Invalid conversion to boolean",linkedValue->getCodeReference());
                }
            }
            else
            {
                switch ( RPNStack.top()->getDataTypeId() )
                {
                case DataTypesId::Numeric:
                {
                    double op1, op2 = *(double*)RPNStack.top()->getValue();
                    bool isBoolean = false,bop1, bop2 = op2;

                    delete RPNStack.top();
                    RPNStack.pop();

                    if( RPNStack.empty() )
                        throw SyntaxException("Invalid operation",linkedValue->getCodeReference());

                    switch( RPNStack.top()->getDataTypeId() )
                    {
                    case DataTypesId::Numeric:
                        op1 = *(double*)RPNStack.top()->getValue();
                        break;
                    case DataTypesId::Boolean:
                        isBoolean = true;
                        bop1 = *(bool*)RPNStack.top()->getValue();
                        break;
                    default:
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException("Invalid conversion",linkedValue->getCodeReference());
                        break;
                    }

                    delete RPNStack.top();
                    RPNStack.pop();

                    switch( *(char*)literalValue->getValue() )
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
                            while(!RPNStack.empty())
                            {
                                delete RPNStack.top();
                                RPNStack.pop();
                            }
                            throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                        }
                        RPNStack.push(new BooleanLiteralValue(op1>op2));
                        break;
                    case cCast(BooleanSymbols::GreaterEqual):
                        if( isBoolean )
                        {
                            while(!RPNStack.empty())
                            {
                                delete RPNStack.top();
                                RPNStack.pop();
                            }
                            throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                        }
                        RPNStack.push(new BooleanLiteralValue(op1>=op2));
                        break;
                    case cCast(BooleanSymbols::Less):
                        if( isBoolean )
                        {
                            while(!RPNStack.empty())
                            {
                                delete RPNStack.top();
                                RPNStack.pop();
                            }
                            throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                        }
                        RPNStack.push(new BooleanLiteralValue(op1<op2));
                        break;
                    case cCast(BooleanSymbols::LessEqual):
                        if( isBoolean )
                        {
                            while(!RPNStack.empty())
                            {
                                delete RPNStack.top();
                                RPNStack.pop();
                            }
                            throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                        }
                        RPNStack.push(new BooleanLiteralValue(op1<=op2));
                        break;
                    default:
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
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
                        throw SyntaxException("Invalid operation",linkedValue->getCodeReference());

                    switch( RPNStack.top()->getDataTypeId() )
                    {
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
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException("Invalid conversion",linkedValue->getCodeReference());
                        break;
                    }

                    delete RPNStack.top();
                    RPNStack.pop();

                    switch( *(char*)literalValue->getValue() )
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
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
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
                        throw SyntaxException("Invalid operation",linkedValue->getCodeReference());

                    switch( RPNStack.top()->getDataTypeId() )
                    {
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
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException("Invalid conversion",linkedValue->getCodeReference());
                        break;
                    }

                    delete RPNStack.top();
                    RPNStack.pop();

                    switch( *(char*)literalValue->getValue() )
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
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
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
                        throw SyntaxException("Invalid operation",linkedValue->getCodeReference());

                    switch( RPNStack.top()->getDataTypeId() )
                    {
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
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException("Invalid conversion",linkedValue->getCodeReference());
                        break;
                    }

                    delete RPNStack.top();
                    RPNStack.pop();

                    switch( *(char*)literalValue->getValue() )
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
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                        break;
                    }
                }
                break;
                default:
                    while(!RPNStack.empty())
                    {
                        delete RPNStack.top();
                        RPNStack.pop();
                    }
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
            while(!RPNStack.empty())
            {
                delete RPNStack.top();
                RPNStack.pop();
            }
            throw SemanticException(errorDescription,linkedValue->getCodeReference());
            break;
        }
    }

    if ( RPNStack.empty() )
        throw SyntaxException("Invalid number of operands",this->getCodeReference());

    bool ret = *(bool*)RPNStack.top()->getValue();

    delete RPNStack.top();
    RPNStack.pop();

    if ( !RPNStack.empty() )
    {
        while(!RPNStack.empty())
        {
            delete RPNStack.top();
            RPNStack.pop();
        }
        throw SyntaxException("Invalid number of operands",this->getCodeReference());
    }
    return new BooleanLiteralValue(ret);
}
