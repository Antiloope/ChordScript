#include "instructionexpression.h"
#include "interpreter/context.h"
#include "utils/Exceptions/exception.h"
#include "terminalexpression.h"

using namespace CS;
using namespace std;

InstructionExpression::InstructionExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void InstructionExpression::load(
    list<TerminalExpression*>* terminalExpressionsList) {

    Context* ctx = Context::getInstance();

    auto aux = terminalExpressionsList->front();
    auto codeReference = aux->getCodeReference();

    switch( aux->getType() )
    {
    case cCast(TerminalExpressionType::For):
        _instruction = new ForInstructionExpression(codeReference);
        break;
    case cCast(TerminalExpressionType::If):
        _instruction = new IfInstructionExpression(codeReference);
        break;
    case cCast(TerminalExpressionType::Break):
        _instruction = new BreakInstructionExpression(codeReference);
        break;
    case cCast(TerminalExpressionType::EOE):
        _instruction = new EOEInstructionExpression(codeReference);
        break;
    case cCast(TerminalExpressionType::Return):
        _instruction = new ReturnInstructionExpression(codeReference);
        break;
    case cCast(TerminalExpressionType::Name):
    {
        auto nameExpression = (NameExpression*)aux;

        // If its a data type, this line is an assignation or a definition
        if( ctx->isDataType( DataType::getDataTypeId(nameExpression->getName()) ) )
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if( it == terminalExpressionsList->end() )
                throw SyntaxException("Expected a variable or function name",codeReference);

            aux = *it;
            // This element must be a name
            if(
                aux->getType() == cCast(TerminalExpressionType::Name) &&
                ctx->isValidName( ((NameExpression*)aux)->getName() ) )
            {
                advance(it,1);
                if( it == terminalExpressionsList->end())
                    throw SyntaxException("Expected an assignation or a definition",aux->getCodeReference());

                aux = *it;

                if( aux->getType() == cCast(TerminalExpressionType::Equal) )
                    _instruction = new AssignationExpression(aux->getCodeReference());
                else if(
                    aux->getType() == cCast(TerminalExpressionType::EOE) ||
                    aux->getType() == cCast(TerminalExpressionType::OpenParenthesis))
                    _instruction = new DefinitionExpression(aux->getCodeReference());
                else
                    throw SyntaxException("Expected an assignation or a definition",aux->getCodeReference());
            }
            else
                throw SemanticException("Invalid name",aux->getCodeReference());
        }
        // If its not a data type, must be a valid variable name
        else if( ctx->isValidName(nameExpression->getName()) )
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if(it == terminalExpressionsList->end())
                throw SyntaxException("Unrecognized variable name or data type",aux->getCodeReference());

            aux = *it;

            if( aux->getType() == cCast(TerminalExpressionType::Equal) )
                _instruction = new AssignationExpression(aux->getCodeReference());
            else if(
                aux->getType() == cCast(TerminalExpressionType::MemberAccess) ||
                aux->getType() == cCast(TerminalExpressionType::OpenParenthesis) )

                _instruction = new ExecutionExpression(aux->getCodeReference());
            else
                throw SyntaxException("Unrecognized variable name or data type",aux->getCodeReference());
        }
        else
            throw SyntaxException("Unrecognized variable name or data type",aux->getCodeReference());
        break;
    }
    default:
        throw SyntaxException("Invalid instruction",codeReference);
    }

    _instruction->load( terminalExpressionsList );
}

InstructionExpression::~InstructionExpression() {
    if( _instruction )
        delete _instruction;
}

void InstructionExpression::interpret() {
    _instruction->interpret();
}
