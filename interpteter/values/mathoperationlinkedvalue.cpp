#include "mathoperationlinkedvalue.h"
#include "interpteter/expressions/terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpteter/values/linkedvalue.h"
#include "interpteter/values/literalvalue.h"
#include "interpteter/aux/soundgenerator.h"
#include "interpteter/aux/sampleplayer.h"
#include <stack>
#include <memory>
#include "interpteter/context.h"

using namespace CS;
using namespace std;

MathOperationLinkedValue::MathOperationLinkedValue(size_t codeReference) :
    LinkedValue(codeReference) {}

void MathOperationLinkedValue::load(
    list<TerminalExpression*>* terminalExpressionsList
    ) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    stack<char> RPNStack;

    //Check if first element of operation is a plus or minus sign to consider it or not in the operation
    if ( tmp->getType() == cCast(TerminalExpressionType::Addition) )
        terminalExpressionsList->pop_front();
    else if ( tmp->getType() == cCast(TerminalExpressionType::Substract) )
    {
        _linkedValuesList.push_back(new NumberLinkedValue(tmp->getCodeReference()));
        TerminalExpression* minus = new NumericExpression(tmp->getCodeReference(),cCast(TerminalExpressionType::Numeric),-1);
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
        case cCast(TerminalExpressionType::OpenParenthesis):
            RPNStack.push(cCast(MathSymbols::OpenParenthesis));
            break;
        case cCast(TerminalExpressionType::Addition):
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
        case cCast(TerminalExpressionType::Substract):
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
        case cCast(TerminalExpressionType::Multiplication):
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
        case cCast(TerminalExpressionType::Divition):
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
        case cCast(TerminalExpressionType::Numeric):
            _linkedValuesList.push_back(new NumberLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            terminalExpressionsList->push_front(nullptr);
            break;
        case cCast(TerminalExpressionType::CloseParenthesis):
            while( !RPNStack.empty() && RPNStack.top() != cCast(MathSymbols::OpenParenthesis) )
            {
                auto op = new OperatorLinkedValue(tmp->getCodeReference());
                _linkedValuesList.push_back(op);
                op->load(RPNStack.top());
                RPNStack.pop();
            }
            if ( !RPNStack.empty() && RPNStack.top() == cCast(MathSymbols::OpenParenthesis) )
                RPNStack.pop();
            if ( RPNStack.empty() )
                isValidExpression = false;
            break;
        case cCast(TerminalExpressionType::Name):
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);

            if( it == terminalExpressionsList->end() )
                throw SyntaxException("Expected another symbol",tmp->getCodeReference());

            tmp = *it;
            if (
                tmp->getType() == cCast(TerminalExpressionType::MemberAccess) ||
                tmp->getType() == cCast(TerminalExpressionType::OpenParenthesis) )
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
                    throw SemanticException("Unrecognized name", tmp->getCodeReference());
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
            throw SyntaxException("Expected )",tmp->getCodeReference());
        else
            throw SyntaxException("Invalid symbol at mathematical operation",tmp->getCodeReference());
    }
}

MathOperationLinkedValue::~MathOperationLinkedValue() {
    while( !_linkedValuesList.empty() ) {
        if( _linkedValuesList.front() )
            delete _linkedValuesList.front();
        _linkedValuesList.pop_front();
    }
}

LiteralValue* MathOperationLinkedValue::getValue() const {
    stack<LiteralValue*> RPNStack;

    for( LinkedValue* linkedValue : _linkedValuesList )
    {
        unique_ptr<LiteralValue> literalValue = unique_ptr<LiteralValue>(linkedValue->getValue());

        switch (literalValue->getDataTypeId())
        {
        case DataTypesId::Numeric:
        case DataTypesId::Sound:
        case DataTypesId::String:
            RPNStack.push(literalValue->clone());
            break;
        case DataTypesId::Operator:
        {
            if( RPNStack.empty() )
                throw SyntaxException("Invalid operation",linkedValue->getCodeReference());

            switch( RPNStack.top()->getDataTypeId() )
            {
            case DataTypesId::Numeric:
            {
                double op2 = *(double*)RPNStack.top()->getValue();

                delete RPNStack.top();
                RPNStack.pop();

                if( RPNStack.empty() )
                    throw SyntaxException("Invalid operation",linkedValue->getCodeReference());

                switch( *(char*)literalValue->getValue() )
                {
                case cCast(MathSymbols::Addition):
                    switch ( RPNStack.top()->getDataTypeId() )
                    {
                    case DataTypesId::Numeric:
                    {
                        double op1 = *(double*)RPNStack.top()->getValue();

                        delete RPNStack.top();
                        RPNStack.pop();

                        RPNStack.push(new NumberLiteralValue(op1 + op2));
                        break;
                    }
                    default:
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException(
                            "Invalid addition operation between a number and a " + DataType::getDataTypeString(RPNStack.top()->getDataTypeId()),
                            linkedValue->getCodeReference());
                        break;
                    }
                    break;
                case cCast(MathSymbols::Substraction):
                    switch ( RPNStack.top()->getDataTypeId() )
                    {
                    case DataTypesId::Numeric:
                    {
                        double op1 = *(double*)RPNStack.top()->getValue();

                        delete RPNStack.top();
                        RPNStack.pop();

                        RPNStack.push(new NumberLiteralValue(op1 - op2));
                        break;
                    }
                    default:
                        DataTypesId dataTypeId = RPNStack.top()->getDataTypeId();
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException(
                            "Invalid substraction operation between a number and a " + DataType::getDataTypeString(dataTypeId),
                            linkedValue->getCodeReference());
                        break;
                    }
                    break;
                case cCast(MathSymbols::Multiplication):
                    switch ( RPNStack.top()->getDataTypeId() )
                    {
                    case DataTypesId::Numeric:
                    {
                        double op1 = *(double*)RPNStack.top()->getValue();

                        delete RPNStack.top();
                        RPNStack.pop();

                        RPNStack.push(new NumberLiteralValue(op1 * op2));
                        break;
                    }
                    case DataTypesId::Sound:
                    {
                        unique_ptr<SoundGenerator> op1 = unique_ptr<SoundGenerator>(((SoundGenerator*)RPNStack.top()->getValue())->clone());

                        delete RPNStack.top();
                        RPNStack.pop();

                        RPNStack.push(new SoundLiteralValue(op1->operator*(op2).clone()));
                        break;
                    }
                    default:
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException(
                            "Invalid multiplication operation between a number and a " + DataType::getDataTypeString(RPNStack.top()->getDataTypeId()),
                            linkedValue->getCodeReference());
                        break;
                    }
                    break;
                case cCast(MathSymbols::Divition):
                    switch ( RPNStack.top()->getDataTypeId() )
                    {
                    case DataTypesId::Numeric:
                    {
                        double op1 = *(double*)RPNStack.top()->getValue();
                        delete RPNStack.top();
                        RPNStack.pop();
                        RPNStack.push(new NumberLiteralValue(op1 / op2));
                        break;
                    }
                    default:
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException(
                            "Invalid divition operation between a number and a " + DataType::getDataTypeString(RPNStack.top()->getDataTypeId()),
                            linkedValue->getCodeReference());
                        break;
                    }
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
                break;
            }
            case DataTypesId::String:
            {
                string op2 = *(string*)RPNStack.top()->getValue();

                delete RPNStack.top();
                RPNStack.pop();

                if( RPNStack.empty() )
                    throw SyntaxException("Invalid operation",linkedValue->getCodeReference());

                switch( *(char*)literalValue->getValue() )
                {
                case cCast(MathSymbols::Addition):
                    switch ( RPNStack.top()->getDataTypeId() )
                    {
                    case DataTypesId::String:
                    {
                        string op1 = *(string*)RPNStack.top()->getValue();

                        delete RPNStack.top();
                        RPNStack.pop();

                        RPNStack.push(new StringLiteralValue(op1 + op2));
                        break;
                    }
                    default:
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException(
                            "Invalid addition operation between a string and a " + DataType::getDataTypeString(RPNStack.top()->getDataTypeId()),
                            linkedValue->getCodeReference());
                        break;
                    }
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
                break;
            }
            case DataTypesId::Sound:
            {
                unique_ptr<SoundGenerator> op2 = unique_ptr<SoundGenerator>(((SoundGenerator*)RPNStack.top()->getValue())->clone());

                delete RPNStack.top();
                RPNStack.pop();

                if( RPNStack.empty() )
                    throw SyntaxException("Invalid operation",linkedValue->getCodeReference());

                switch( *(char*)literalValue->getValue() )
                {
                case cCast(MathSymbols::Addition):
                    switch ( RPNStack.top()->getDataTypeId() )
                    {
                    case DataTypesId::Sound:
                    {
                        unique_ptr<SoundGenerator> op1(((SoundGenerator*)RPNStack.top()->getValue())->clone());

                        delete RPNStack.top();
                        RPNStack.pop();

                        RPNStack.push(new SoundLiteralValue((*op1 + *op2).clone()));
                        break;
                    }
                    default:
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException(
                            "Invalid addition operation between a sound and a " + DataType::getDataTypeString(RPNStack.top()->getDataTypeId()),
                            linkedValue->getCodeReference());
                        break;
                    }
                    break;
                case cCast(MathSymbols::Multiplication):
                {
                    switch ( RPNStack.top()->getDataTypeId() )
                    {
                    case DataTypesId::Numeric:
                    {
                        double op1 = *(double*)RPNStack.top()->getValue();

                        delete RPNStack.top();
                        RPNStack.pop();

                        RPNStack.push(new SoundLiteralValue(op2->operator*(op1).clone()));
                        break;
                    }
                    case DataTypesId::Sound:
                    {
                        unique_ptr<SoundGenerator> op1 = unique_ptr<SoundGenerator>(((SoundGenerator*)RPNStack.top()->getValue())->clone());

                        delete RPNStack.top();
                        RPNStack.pop();

                        RPNStack.push(new SoundLiteralValue(op1->operator*(*op2).clone()));
                        break;
                    }
                    default:
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException(
                            "Invalid multiplication operation between a sound and a " + DataType::getDataTypeString(RPNStack.top()->getDataTypeId()),
                            linkedValue->getCodeReference());
                        break;
                    }
                    break;
                }
                case cCast(MathSymbols::Divition):
                {
                    switch ( RPNStack.top()->getDataTypeId() )
                    {
                    case DataTypesId::Numeric:
                    {
                        double op1 = *(double*)RPNStack.top()->getValue();

                        delete RPNStack.top();
                        RPNStack.pop();

                        RPNStack.push(new SoundLiteralValue(op2->operator/(op1).clone()));
                        break;
                    }
                    default:
                        while(!RPNStack.empty())
                        {
                            delete RPNStack.top();
                            RPNStack.pop();
                        }
                        throw SyntaxException(
                            "Invalid divition operation between a number and a " + DataType::getDataTypeString(RPNStack.top()->getDataTypeId()),
                            linkedValue->getCodeReference());
                        break;
                    }
                    break;
                }
                default:
                    while(!RPNStack.empty())
                    {
                        delete RPNStack.top();
                        RPNStack.pop();
                    }
                    throw SyntaxException("Invalid operation",linkedValue->getCodeReference());
                    break;
                }
                break;
            }
            default:
                while(!RPNStack.empty())
                {
                    delete RPNStack.top();
                    RPNStack.pop();
                }
                throw SyntaxException("Unknown literal value",linkedValue->getCodeReference());
                break;
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

    if ( RPNStack.empty() )
        throw SyntaxException("Invalid number of operands",this->getCodeReference());

    LiteralValue* ret = RPNStack.top();
    RPNStack.pop();

    if ( !RPNStack.empty() )
        throw SyntaxException("Invalid number of operands",this->getCodeReference());

    return ret;
}
