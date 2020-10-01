#ifndef LINKEDVALUE_H
#define LINKEDVALUE_H

#include <list>
#include "value.h"

namespace CS {

class TerminalExpression;

class LinkedValue : public Value {
public:
    LinkedValue(size_t);
    virtual LiteralValue* getValue() const = 0;
    virtual void load(std::list<TerminalExpression*>*) = 0;
    virtual ~LinkedValue();
    static LinkedValue* generateLinkedValue(std::list<TerminalExpression*>*);
    static LinkedValue* detectOperation(std::list<TerminalExpression*>*);
    size_t getCodeReference() const;
private:
    size_t _codeReference;
};

}

#include "mathoperationlinkedvalue.h"
#include "nulllinkedvalue.h"
#include "stringlinkedvalue.h"
#include "booleanoperationlinkedvalue.h"
#include "arraylinkedvalue.h"
#include "executionlinkedvalue.h"
#include "namelinkedvalue.h"
#include "numberlinkedvalue.h"
#include "booleanlinkedvalue.h"
#include "operatorlinkedvalue.h"

#endif // LINKEDVALUE_H
