#include "breakinstructionexpression.h"
#include "interpteter/context.h"
#include "utils/Exceptions/exception.h"
#include "terminalexpression.h"

using namespace CS;
using namespace std;

BreakInstructionExpression::BreakInstructionExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

void BreakInstructionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    if(Context::getInstance()->getCurrentScope() == GLOBAL_SCOPE)
        throw SyntaxException("Break is not valid in global scope",terminalExpressionsList->front()->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty() ||
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::EOE)
        )
        throw SyntaxException("Expected ;",terminalExpressionsList->front()->getCodeReference());
}

BreakInstructionExpression::~BreakInstructionExpression() {}

void BreakInstructionExpression::interpret() {
    Context::getInstance()->setReturnValue();
}
