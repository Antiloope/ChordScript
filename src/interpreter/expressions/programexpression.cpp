#include "programexpression.h"
#include "terminalexpression.h"
#include "interpreter/context.h"

using namespace CS;
using namespace std;

ProgramExpression::ProgramExpression() : NonTerminalExpression(0) {}

ProgramExpression::ProgramExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void ProgramExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    auto firstExpression = terminalExpressionsList->front();

    // Load instructions while list is not empty and next expression
    // is not a close brace
    while (
        !terminalExpressionsList->empty() &&
        firstExpression->getType() != cCast(TerminalExpressionType::CloseBrace) )
    {
        auto instruction = new InstructionExpression(firstExpression->getCodeReference());
        _instructionsList.push_back( instruction );
        instruction->load( terminalExpressionsList );

        // Assign next expression
        firstExpression = terminalExpressionsList->front();
    }
}

void ProgramExpression::interpret(list<TerminalExpression*> *terminalExpressionsList) {
    // Load and interpret all instructions
    while( !terminalExpressionsList->empty() )
    {
        InstructionExpression tmp(
            terminalExpressionsList->front()->getCodeReference() );

        tmp.load( terminalExpressionsList );
        tmp.interpret();
    }
}

void ProgramExpression::interpret() {
    Context::getInstance()->setReturnValue(nullptr);

    // Interpret all loaded instructions until a return is reached
    for( auto instruction : _instructionsList )
    {
        instruction->interpret();
        if( Context::getInstance()->existReturnValue() )
            break;
    }
}

ProgramExpression::~ProgramExpression() {
    while( !_instructionsList.empty() )
    {
        InstructionExpression* aux = _instructionsList.front();
        delete aux;
        _instructionsList.pop_front();
    }
}
