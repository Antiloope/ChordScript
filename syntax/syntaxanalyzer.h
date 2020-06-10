#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include <string>
#include <queue>
#include "expression.h"
#include "utils/Exceptions/exception.h"

using namespace std;

namespace CS {

class AnalizerResponse
{
public:
    AnalizerResponse();
    void setResponse(const SyntaxException e);
    void setResponse(const string message);
private:
    string _message = "";
    size_t _charReference = 0;
};

class SyntaxAnalyzer
{
public:
    SyntaxAnalyzer();
    ~SyntaxAnalyzer();

    AnalizerResponse analize(const string sourceCode);
    AnalizerResponse interpret(const string sourceCode);
private:
    queue<Expression*> _expressionsQueue;
};

}

#endif // SYNTAXANALYZER_H
