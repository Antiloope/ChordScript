#include "linkedvalue.h"
#include "utils/Exceptions/exception.h"
#include "interpteter/expressions/terminalexpression.h"

using namespace CS;
using namespace std;

LinkedValue::LinkedValue(size_t codeReference) :
    Value(DataTypesId::Null),
    _codeReference(codeReference) {}

LinkedValue::~LinkedValue() {}

LinkedValue* LinkedValue::generateLinkedValue(
    list<TerminalExpression*>* terminalExpressionsList
    ){
    if( terminalExpressionsList->empty() )
        throw SyntaxException("Expected a value");

    TerminalExpression* tmp = terminalExpressionsList->front();

    LinkedValue* ret;

    switch ( tmp->getType() )
    {
    case cCast(TerminalExpressionType::String):
        ret = new StringLinkedValue(tmp->getCodeReference());
        break;
    case cCast(TerminalExpressionType::Null):
        ret = new NullLinkedValue(tmp->getCodeReference());
        break;
    case cCast(TerminalExpressionType::Addition):
    case cCast(TerminalExpressionType::Substract):
        ret = new MathOperationLinkedValue(tmp->getCodeReference());
        break;
    case cCast(TerminalExpressionType::OpenBracket):
    case cCast(TerminalExpressionType::OpenParenthesis):
        ret = new ArrayLinkedValue(tmp->getCodeReference());
        break;
    case cCast(TerminalExpressionType::Boolean):
    case cCast(TerminalExpressionType::Negation):
        ret = new BooleanOperationLinkedValue(tmp->getCodeReference());
        break;
    case cCast(TerminalExpressionType::Name):
    {
        auto it = terminalExpressionsList->begin();
        advance(it,1);

        if( it == terminalExpressionsList->end() )
            throw SyntaxException("Expected another symbol",tmp->getCodeReference());

        tmp = *it;
        switch (tmp->getType()) {
        case cCast(TerminalExpressionType::Addition):
        case cCast(TerminalExpressionType::Substract):
        case cCast(TerminalExpressionType::Divition):
        case cCast(TerminalExpressionType::Multiplication):
            ret = new MathOperationLinkedValue(tmp->getCodeReference());
            break;
        case cCast(TerminalExpressionType::Equal):
        case cCast(TerminalExpressionType::GreaterThan):
        case cCast(TerminalExpressionType::LessThan):
        case cCast(TerminalExpressionType::Negation):
            ret = new BooleanOperationLinkedValue(tmp->getCodeReference());
            break;
        default:
            ret = detectOperation(terminalExpressionsList);
            break;
        }
        break;
    }
    case cCast(TerminalExpressionType::Numeric):
    {
        auto it = terminalExpressionsList->begin();
        advance(it,1);

        if(it == terminalExpressionsList->end())
            throw SyntaxException("Expected another symbol",tmp->getCodeReference());

        tmp = *it;
        if(
            tmp->getType() == cCast(TerminalExpressionType::Equal) ||
            tmp->getType() == cCast(TerminalExpressionType::GreaterThan) ||
            tmp->getType() == cCast(TerminalExpressionType::LessThan)||
            tmp->getType() == cCast(TerminalExpressionType::Negation)
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

LinkedValue* LinkedValue::detectOperation(
    list<TerminalExpression*>* terminalExpressionsList
    ) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    LinkedValue* ret;

    size_t codeReference = tmp->getCodeReference();

    auto it = terminalExpressionsList->begin();
    advance(it,1);

    if( it == terminalExpressionsList->end() )
        throw SyntaxException("Expected another symbol",tmp->getCodeReference());

    tmp = *it;

    if( tmp->getType() == cCast(TerminalExpressionType::MemberAccess) )
    {
        advance(it,1);
        if( it == terminalExpressionsList->end() )
            throw SyntaxException("Expected another symbol",tmp->getCodeReference());

        tmp = *it;

        if( tmp->getType() != cCast(TerminalExpressionType::Name) )
            throw SyntaxException("Expected a method name",tmp->getCodeReference());

        advance(it,1);

        if( it == terminalExpressionsList->end() )
            throw SyntaxException("Expected another symbol",tmp->getCodeReference());

        tmp = *it;

        if( tmp->getType() != cCast(TerminalExpressionType::OpenParenthesis) ) throw SyntaxException("Expected open pharentesis",tmp->getCodeReference());
    }

    if( tmp->getType() == cCast(TerminalExpressionType::OpenParenthesis) ) {
        size_t pharentesisCounter = 1;
        while(1)
        {
            advance(it,1);

            if( it == terminalExpressionsList->end() )
                throw SyntaxException("Expected another symbol",tmp->getCodeReference());

            tmp = *it;

            if( tmp->getType() == cCast(TerminalExpressionType::OpenParenthesis) )
                pharentesisCounter++;

            else if( tmp->getType() == cCast(TerminalExpressionType::CloseParenthesis) )
            {
                pharentesisCounter--;
                if(pharentesisCounter == 0)
                {
                    advance(it,1);

                    if( it == terminalExpressionsList->end() )
                        throw SyntaxException("Expected another symbol",tmp->getCodeReference());

                    tmp = *it;

                    switch (tmp->getType()) {
                    case cCast(TerminalExpressionType::Addition):
                    case cCast(TerminalExpressionType::Substract):
                    case cCast(TerminalExpressionType::Divition):
                    case cCast(TerminalExpressionType::Multiplication):
                        ret = new MathOperationLinkedValue(codeReference);
                        break;
                    case cCast(TerminalExpressionType::Equal):
                    case cCast(TerminalExpressionType::GreaterThan):
                    case cCast(TerminalExpressionType::LessThan):
                    case cCast(TerminalExpressionType::Negation):
                    case cCast(TerminalExpressionType::Or):
                    case cCast(TerminalExpressionType::And):
                        ret = new BooleanOperationLinkedValue(codeReference);
                        break;
                    default:
                        ret = new ExecutionLinkedValue(codeReference);
                        break;
                    }
                    break;
                }
            }
            else if( tmp->getType() == cCast(TerminalExpressionType::EOE) )
                throw SyntaxException("Expected a closed pharentesis",tmp->getCodeReference());
        }
    }
    else
        ret = new NameLinkedValue(codeReference);

    return ret;
}

size_t LinkedValue::getCodeReference() const {
    return _codeReference;
}
