#include "instructionexpression.h"
#include "interpreter/context.h"
#include "utils/Exceptions/exception.h"
#include "terminalexpression.h"

using namespace CS;
using namespace std;

InstructionExpression::InstructionExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

InstructionExpression::InstructionExpression(const InstructionExpression& e) : NonTerminalExpression(e.getCodeReference()) {
    _instruction = e._instruction;
}

void InstructionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    switch (tmp->getType())
    {
    case cCast(TerminalExpressionType::For):
        _instruction = new ForInstructionExpression(tmp->getCodeReference());
        break;
    case cCast(TerminalExpressionType::If):
        _instruction = new IfInstructionExpression(tmp->getCodeReference());
        break;
    case cCast(TerminalExpressionType::Break):
        _instruction = new BreakInstructionExpression(tmp->getCodeReference());
        break;
    case cCast(TerminalExpressionType::EOE):
        _instruction = new EOEInstructionExpression(tmp->getCodeReference());
        break;
    case cCast(TerminalExpressionType::Return):
        _instruction = new ReturnInstructionExpression(tmp->getCodeReference());
        break;
    case cCast(TerminalExpressionType::Name):
    {
        NameExpression* nameExpression = (NameExpression*)tmp;
        if ( Context::getInstance()->isDataType(DataType::getDataTypeId(nameExpression->getName())) )
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if(it == terminalExpressionsList->end()) throw SyntaxException("Expected variable name",tmp->getCodeReference());
            tmp = *it;
            if (
                tmp->getType() == cCast(TerminalExpressionType::Name) &&
                Context::getInstance()->isValidName(((NameExpression*)tmp)->getName()))
            {
                advance(it,1);
                if( it == terminalExpressionsList->end()) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
                tmp = *it;
                if( tmp->getType() == cCast(TerminalExpressionType::Equal) )
                {
                    _instruction = new AssignationExpression(tmp->getCodeReference());
                }
                else if(
                    tmp->getType() == cCast(TerminalExpressionType::EOE) ||
                    tmp->getType() == cCast(TerminalExpressionType::OpenParenthesis))
                {
                    _instruction = new DefinitionExpression(tmp->getCodeReference());
                }
                else
                {
                    throw SyntaxException("Expected another symbol",tmp->getCodeReference());
                }
            }
            else
            {
                throw SemanticException("Invalid name",tmp->getCodeReference());
            }
        }
        else if( Context::getInstance()->isValidName(nameExpression->getName()) )
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if(it == terminalExpressionsList->end()) throw SyntaxException("Expected variable name",tmp->getCodeReference());
            tmp = *it;
            if( tmp->getType() == cCast(TerminalExpressionType::Equal) )
            {
                _instruction = new AssignationExpression(tmp->getCodeReference());
            }
            else if(
                tmp->getType() == cCast(TerminalExpressionType::MemberAccess) ||
                tmp->getType() == cCast(TerminalExpressionType::OpenParenthesis) )
            {
                _instruction = new ExecutionExpression(tmp->getCodeReference());
            }
            else
            {
                throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            }
        }
        else
        {
            throw SyntaxException("Unrecognized variable name or data type",tmp->getCodeReference());
        }
        break;
    }
    default:
        throw SyntaxException("Expected another instruction",tmp->getCodeReference());
    }

    _instruction->load(terminalExpressionsList);
}

InstructionExpression::~InstructionExpression(){
    if( _instruction ) delete _instruction;
}

void InstructionExpression::interpret(){
    _instruction->interpret();
}
