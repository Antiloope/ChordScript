#include "syntaxanalyzer.h"

#include "lexer.h"
#include "utils/Exceptions/exception.h"

using namespace CS;

AnalizerResponse::AnalizerResponse()
{

}

void AnalizerResponse::setResponse(const SyntaxException e)
{
    _message = e.what();
    _charReference = e.getCharacterRefference();
}

void AnalizerResponse::setResponse(const string message)
{
    _message = message;
}

SyntaxAnalyzer::SyntaxAnalyzer()
{

}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
    Expression* tmp;
    while (_expressionsQueue.front()) {
        tmp = _expressionsQueue.front();
        _expressionsQueue.pop();
        delete tmp;
    }
}

AnalizerResponse SyntaxAnalyzer::analize(const string sourceCode)
{
    AnalizerResponse ret;
    try {
        _expressionsQueue = tokenize(sourceCode);
    } catch (const SyntaxException& e) {
        ret.setResponse(e);
    } catch (const exception& e) {
        ret.setResponse(e.what());
    }

    return ret;
}

AnalizerResponse SyntaxAnalyzer::interpret(const string sourceCode)
{
    AnalizerResponse ret;
    return ret;
}
