#ifndef NAMELINKEDVALUE_H
#define NAMELINKEDVALUE_H

#include "linkedvalue.h"

namespace CS {

class NameLinkedValue : public LinkedValue {
public:
    NameLinkedValue(size_t);
    void load(std::list<TerminalExpression*>*) override;
    LiteralValue * getValue() const override;
private:
    std::string _name;
};

}

#endif // NAMELINKEDVALUE_H
