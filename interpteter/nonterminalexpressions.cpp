#include "nonterminalexpressions.h"
#include "utils/Exceptions/exception.h"
#include "context.h"

using namespace CS;

NonTerminalExpression::NonTerminalExpression() : Expression() {}

NonTerminalExpression::NonTerminalExpression(size_t codeReference) : Expression(codeReference) {}

NonTerminalExpression::~NonTerminalExpression(){}

/// Program expression

ProgramExpression::ProgramExpression(list<TerminalExpression*> expressionsList) : NonTerminalExpression(0){
    while (!expressionsList.empty())
    {
        _instructionsList.push_back(new InstructionExpression(&expressionsList,expressionsList.front()->getCodeReference()));
    }
}

ProgramExpression::ProgramExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
    TerminalExpression* tmp = expressionsList->front();
    while (
        (typeid (*tmp).hash_code() != typeid (ReturnExpression).hash_code() &&
         typeid (*tmp).hash_code() != typeid (CloseBraceExpression).hash_code()) &&
        !expressionsList->empty())
    {
        _instructionsList.push_back(new InstructionExpression(expressionsList,tmp->getCodeReference()));
        tmp = expressionsList->front();
    }
}

ProgramExpression::~ProgramExpression(){
    InstructionExpression* tmp;
    while(!_instructionsList.empty())
    {
        tmp = _instructionsList.front();
        _instructionsList.pop_front();
        delete tmp;
    }
}

void ProgramExpression::interpret(){

}

/// Instruction expression

InstructionExpression::InstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
    if(expressionsList->empty()) throw SyntaxException("Expected another instruction",codeReference);

    TerminalExpression* tmp = expressionsList->front();

    if(typeid (*tmp).hash_code() == typeid (ForExpression).hash_code()){
        _instruction = new ForInstructionExpression(expressionsList,codeReference);
    }
    else if(typeid (*tmp).hash_code() == typeid (IfExpression).hash_code()){
        _instruction = new IfInstructionExpression(expressionsList,codeReference);
    }
    else if(typeid (*tmp).hash_code() == typeid (BreakExpression).hash_code()){
        _instruction = new BreakInstructionExpression(expressionsList,codeReference);
    }
    else if(typeid (*tmp).hash_code() == typeid (EOEExpression).hash_code()){
        _instruction = new EOEInstructionExpression(expressionsList,codeReference);
    }
    else if(typeid (*tmp).hash_code() == typeid (NameExpression).hash_code()){
        NameExpression* nameExpression = (NameExpression*)tmp;
        if(Context::getInstance()->isDataType(nameExpression->getName())){
            auto it = expressionsList->begin();
            advance(it,1);
            if(it == expressionsList->end()) throw SyntaxException("Expected variable name",codeReference);
            tmp = *it;
            if( typeid (*tmp).hash_code() == typeid (NameExpression).hash_code() &&
                Context::getInstance()->isValidName(((NameExpression*)tmp)->getName())){
                advance(it,1);
                if(it == expressionsList->end()) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
                tmp = *it;
                if(typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code()){
                    _instruction = new AssignationExpression(expressionsList,codeReference);
                }
                else if(
                    typeid (*tmp).hash_code() == typeid (EOEExpression).hash_code() ||
                    typeid (*tmp).hash_code() == typeid (OpenParenthesisExpression).hash_code()){
                    _instruction = new DefinitionExpression(expressionsList,codeReference);
                }
                else{
                    throw SyntaxException("Expected another symbol",tmp->getCodeReference());
                }
            }
            else{
                throw SemanticException("Invalid name",tmp->getCodeReference());
            }
        }
        else if(Context::getInstance()->nameExist(nameExpression->getName())){
            auto it = expressionsList->begin();
            advance(it,1);
            if(it == expressionsList->end()) throw SyntaxException("Expected variable name",codeReference);
            tmp = *it;
            if( typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code()){
                _instruction = new AssignationExpression(expressionsList,codeReference);
            }
            else if(
                typeid (*tmp).hash_code() == typeid (MemberAccessExpression).hash_code() ||
                typeid (*tmp).hash_code() == typeid (OpenParenthesisExpression).hash_code()){
                _instruction = new ExecutionExpression(expressionsList,codeReference);
            }
            else{
                throw SyntaxException("Expected another symbol",codeReference);
            }
        }else{
            throw SyntaxException("Unrecognized variable name or data type",codeReference);
        }
    }
    else{
        throw SyntaxException("Expected another instruction",codeReference);
    }
}

InstructionExpression::~InstructionExpression(){
    delete _instruction;
}

void InstructionExpression::interpret(){

}

/// For loop expression

ForInstructionExpression::ForInstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
}

ForInstructionExpression::~ForInstructionExpression(){

}

void ForInstructionExpression::interpret(){

}

/// If expression

IfInstructionExpression::IfInstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
}

IfInstructionExpression::~IfInstructionExpression(){

}

void IfInstructionExpression::interpret(){

}

/// Break expression

BreakInstructionExpression::BreakInstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
}

BreakInstructionExpression::~BreakInstructionExpression(){}

void BreakInstructionExpression::interpret(){

}

/// EOE expression

EOEInstructionExpression::EOEInstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){}

EOEInstructionExpression::~EOEInstructionExpression(){}

void EOEInstructionExpression::interpret(){}

/// Assignation expression

AssignationExpression::AssignationExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){}

AssignationExpression::~AssignationExpression(){}

void AssignationExpression::interpret(){}

/// Definition expression

DefinitionExpression::DefinitionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){}

DefinitionExpression::~DefinitionExpression(){}

void DefinitionExpression::interpret(){}

/// Execution expression

ExecutionExpression::ExecutionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){}

ExecutionExpression::~ExecutionExpression(){}

void ExecutionExpression::interpret(){}
