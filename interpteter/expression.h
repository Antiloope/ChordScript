#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <list>

using namespace std;

namespace CS {

#define cCast(a) static_cast<char>(a)

class Expression {
public:
    Expression();
    Expression(size_t codeReference);
    virtual ~Expression();
    size_t getCodeReference() const;
private:
    size_t _codeReference;
};

}

#endif // EXPRESSION_H
