#include "nonterminalexpressions.h"

using namespace CS;

NonTerminalExpression::NonTerminalExpression() : Expression() {

}
NonTerminalExpression::NonTerminalExpression(size_t codeReference) : Expression(codeReference) {

}
NonTerminalExpression::~NonTerminalExpression(){

}

/// Program expression

ProgramExpression::ProgramExpression(list<TerminalExpression*> expressionsList) : NonTerminalExpression(0){
    expressionsList.pop_front();
    expressionsList.pop_front();
    expressionsList.pop_front();
}

ProgramExpression::ProgramExpression(list<TerminalExpression*>* expressionsList) : NonTerminalExpression(0){

}

ProgramExpression::ProgramExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){

}

ProgramExpression::~ProgramExpression(){
    InstructionExpression* tmp;
    while(!instructionsList.empty())
    {
        tmp = instructionsList.front();
        instructionsList.pop_front();
        delete tmp;
    }
}

void ProgramExpression::interpret(){

}
