#ifndef STRINGLINKEDVALUE_H
#define STRINGLINKEDVALUE_H

#include "linkedvalue.h"

namespace CS {

class StringLinkedValue : public LinkedValue {
public:
    StringLinkedValue(size_t);
    void load(std::list<TerminalExpression*>*) override;
    LiteralValue * getValue() const override;
private:
    std::string _text;
};

}

#endif // STRINGLINKEDVALUE_H
