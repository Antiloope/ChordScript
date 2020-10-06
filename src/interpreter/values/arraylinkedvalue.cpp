#include "arraylinkedvalue.h"
#include "interpreter/expressions/terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/values/literalvalue.h"

using namespace CS;
using namespace std;

ArrayLinkedValue::ArrayLinkedValue(size_t codeReference) :
    LinkedValue(codeReference) {}

void ArrayLinkedValue::load(
    list<TerminalExpression*>* terminalExpressionsList
    ) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    if( tmp->getType() == cCast(TerminalExpressionType::OpenParenthesis) )
        _type = DataTypesId::Argument;
    else if ( tmp->getType() == cCast(TerminalExpressionType::OpenBracket) )
        _type = DataTypesId::Array;
    else
        throw SyntaxException("Expected ( or [",tmp->getCodeReference());

    terminalExpressionsList->pop_front();

    bool isValidValue = true;

    while( isValidValue )
    {
        tmp = terminalExpressionsList->front();
        switch( tmp->getType() )
        {
        case cCast(TerminalExpressionType::String):
            _linkedValuesList.push_back(new StringLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            break;
        case cCast(TerminalExpressionType::Null):
            _linkedValuesList.push_back(new NullLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            break;
        case cCast(TerminalExpressionType::Addition):
        case cCast(TerminalExpressionType::Substract):
            _linkedValuesList.push_back(new MathOperationLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            break;
        case cCast(TerminalExpressionType::Negation):
        case cCast(TerminalExpressionType::Boolean):
            _linkedValuesList.push_back(new BooleanOperationLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            break;
        case cCast(TerminalExpressionType::OpenBracket):
        case cCast(TerminalExpressionType::OpenParenthesis):
            _linkedValuesList.push_back(new ArrayLinkedValue(tmp->getCodeReference()));
            _linkedValuesList.back()->load(terminalExpressionsList);
            break;
        case cCast(TerminalExpressionType::Name):
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);

            if(it == terminalExpressionsList->end())
                throw SyntaxException("Expected another symbol",tmp->getCodeReference());

            tmp = *it;

            switch( tmp->getType() )
            {
            case cCast(TerminalExpressionType::Addition):
            case cCast(TerminalExpressionType::Substract):
            case cCast(TerminalExpressionType::Divition):
            case cCast(TerminalExpressionType::Multiplication):
                _linkedValuesList.push_back(new MathOperationLinkedValue(tmp->getCodeReference()));
                _linkedValuesList.back()->load(terminalExpressionsList);
                break;
            case cCast(TerminalExpressionType::Equal):
            case cCast(TerminalExpressionType::GreaterThan):
            case cCast(TerminalExpressionType::LessThan):
            case cCast(TerminalExpressionType::Negation):
                _linkedValuesList.push_back(new BooleanOperationLinkedValue(tmp->getCodeReference()));
                _linkedValuesList.back()->load(terminalExpressionsList);
                break;
            default:
                _linkedValuesList.push_back(LinkedValue::detectOperation(terminalExpressionsList));
                _linkedValuesList.back()->load(terminalExpressionsList);
                break;
            }
        }
        break;
        case cCast(TerminalExpressionType::Numeric):
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);

            if( it == terminalExpressionsList->end() )
                throw SyntaxException("Expected another symbol",tmp->getCodeReference());

            tmp = *it;

            if(
                tmp->getType() == cCast(TerminalExpressionType::Equal) ||
                tmp->getType() == cCast(TerminalExpressionType::GreaterThan) ||
                tmp->getType() == cCast(TerminalExpressionType::LessThan) ||
                tmp->getType() == cCast(TerminalExpressionType::Negation) )
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
        case cCast(TerminalExpressionType::Separator):
            terminalExpressionsList->pop_front();
            break;
        case cCast(TerminalExpressionType::CloseParenthesis):
        case cCast(TerminalExpressionType::CloseBracket):
            terminalExpressionsList->pop_front();
            isValidValue = false;
            break;
        default:
            throw SyntaxException("Expected a valid value",tmp->getCodeReference());
            break;
        }
    }
}

ArrayLinkedValue::~ArrayLinkedValue() {
    while ( !_linkedValuesList.empty() ) {
        if( _linkedValuesList.front() )
            delete _linkedValuesList.front();
        _linkedValuesList.pop_front();
    }
}

LiteralValue* ArrayLinkedValue::getValue() const {
    list<LiteralValue*> literalValuesList;

    for( LinkedValue* linkedValue : _linkedValuesList )
        literalValuesList.push_back(linkedValue->getValue());

    if( _type == DataTypesId::Array )
        return new ArrayLiteralValue(literalValuesList);

    return new ArgumentLiteralValue(literalValuesList);
}
