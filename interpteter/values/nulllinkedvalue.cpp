#include "nulllinkedvalue.h"
#include "nullliteralvalue.h"
#include "interpteter/expressions/terminalexpression.h"
#include "utils/Exceptions/exception.h"

using namespace CS;
using namespace std;

NullLinkedValue::NullLinkedValue(size_t codeReference) :
    LinkedValue(codeReference) {}

LiteralValue* NullLinkedValue::getValue() const {
    return new NullLiteralValue();
}

void NullLinkedValue::load(
    list<TerminalExpression*>* terminalExpressionsList
    ) {
    if( terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::Null) )
        throw SyntaxException("Expected null", terminalExpressionsList->front()->getCodeReference());

    terminalExpressionsList->pop_front();
}
