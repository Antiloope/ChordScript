#include "programexpression.h"
#include "terminalexpression.h"
#include "interpreter/context.h"

using namespace CS;
using namespace std;

ProgramExpression::ProgramExpression() : NonTerminalExpression(0) {}

ProgramExpression::ProgramExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

void ProgramExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();
    while (
        !terminalExpressionsList->empty() &&
        tmp->getType() != cCast(TerminalExpressionType::CloseBrace) )
    {
        InstructionExpression* instruction = new InstructionExpression(tmp->getCodeReference());
        _instructionsList.push_back(instruction);
        instruction->load(terminalExpressionsList);
        tmp = terminalExpressionsList->front();
    }
}

void ProgramExpression::interpret(list<TerminalExpression*> terminalExpressionsList) {
    while (!terminalExpressionsList.empty())
    {
        InstructionExpression tmp(terminalExpressionsList.front()->getCodeReference());
        tmp.load(&terminalExpressionsList);
        tmp.interpret();
    }
}

void ProgramExpression::interpret(){
    Context::getInstance()->setReturnValue(nullptr);
    for( InstructionExpression* instruction : _instructionsList )
    {
        instruction->interpret();
        if( Context::getInstance()->existReturnValue() )
            break;
    }
}

ProgramExpression::~ProgramExpression() {
    while( !_instructionsList.empty() )
    {
        InstructionExpression* tmp = _instructionsList.front();
        delete tmp;
        _instructionsList.pop_front();
    }
}
