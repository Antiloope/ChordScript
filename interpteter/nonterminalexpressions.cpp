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

AssignationExpression::AssignationExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
    TerminalExpression* tmp = expressionsList->front();
    if(
        typeid (*tmp).hash_code() == typeid (NameExpression).hash_code() &&
        Context::getInstance()->isDataType(((NameExpression*)tmp)->getName()))
    {
        _dataType = ((NameExpression*)tmp)->getName();
        expressionsList->pop_front();
        if(expressionsList->empty()) throw SyntaxException("Expected variable name",tmp->getCodeReference());
        tmp = expressionsList->front();
        if(typeid (*tmp).hash_code() == typeid (NameExpression).hash_code())
        {
            _varName = ((NameExpression*)tmp)->getName();
            expressionsList->pop_front();
            if(expressionsList->empty()) throw SyntaxException("Expected = after name",tmp->getCodeReference());
            tmp = expressionsList->front();
            if(typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code())
            {
                expressionsList->pop_front();
                if(expressionsList->empty()) throw SyntaxException("Expected a value after = ",tmp->getCodeReference());
                tmp = expressionsList->front();
                try {
                    _valueExpression = new ValueExpression(expressionsList,tmp->getCodeReference());
                } catch (SyntaxException& e) {
                    throw e;
                } catch (SemanticException& e) {
                    throw e;
                } catch (exception& e) {
                    throw e;
                }

                Context::getInstance()->newVariable(_varName,_valueExpression->getValue());
            }
            else
            {
                throw SyntaxException("Expected = ",tmp->getCodeReference());
            }
        }
        else
        {
            throw SyntaxException("Expected variable name",tmp->getCodeReference());
        }
    }
    else if(Context::getInstance()->nameExist(((NameExpression*)tmp)->getName()))
    {
        _varName = ((NameExpression*)tmp)->getName();
        _dataType = Context::getInstance()->getDataTypeName(_varName);
        expressionsList->pop_front();
        if(expressionsList->empty()) throw SyntaxException("Expected = after name",tmp->getCodeReference());
        tmp = expressionsList->front();
        if(typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code())
        {
            expressionsList->pop_front();
            if(expressionsList->empty()) throw SyntaxException("Expected a value after = ",tmp->getCodeReference());
            tmp = expressionsList->front();
            try {//TODO: _valueExpression could be used in local scope. Because when assignation is interpreted only name and data type are useful to search in context
                _valueExpression = new ValueExpression(expressionsList,tmp->getCodeReference());
            } catch (SyntaxException& e) {
                throw e;
            } catch (SemanticException& e) {
                throw e;
            } catch (exception& e) {
                throw e;
            }
            Context::getInstance()->newVariable(_varName,_valueExpression->getValue());
        }
        else
        {
            throw SyntaxException("Expected = ",tmp->getCodeReference());
        }
    }
    else
    {
        throw SyntaxException("Expected another symbol",codeReference);
    }
}

AssignationExpression::~AssignationExpression(){
    delete _valueExpression;
}

void AssignationExpression::interpret(){}

/// Definition expression

DefinitionExpression::DefinitionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){}

DefinitionExpression::~DefinitionExpression(){}

void DefinitionExpression::interpret(){}

/// Execution expression

ExecutionExpression::ExecutionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){}

ExecutionExpression::~ExecutionExpression(){}

void ExecutionExpression::interpret(){}

/// Value expression

ValueExpression::ValueExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
    TerminalExpression* tmp = expressionsList->front();
    auto hashCode = typeid (*tmp).hash_code();
    try {
        if(hashCode == typeid (StringExpression).hash_code()){
            _value = new StringLinkedValue((StringExpression*)tmp);
        }
        else if(hashCode == typeid (NullValueExpression).hash_code())
        {
            _value = new NullLinkedValue();
        }
        else if(
            hashCode == typeid (AdditionExpression).hash_code() ||
            hashCode == typeid (SubstractionExpression).hash_code())
        {
            _value = new MathOperationLinkedValue(expressionsList);
        }
        else if(
            hashCode == typeid (NegationExpression).hash_code() ||
            hashCode == typeid (BooleanValueExpression).hash_code())
        {
            _value = new BooleanOperationLinkedValue(expressionsList);
        }
        else if(
            hashCode == typeid (OpenBracketExpression).hash_code() ||
            hashCode == typeid (OpenParenthesisExpression).hash_code())
        {
            _value = new ArrayLinkedValue(expressionsList);
        }
        else if(hashCode == typeid (NameExpression).hash_code())
        {
            auto it = expressionsList->begin();
            advance(it,1);
            if(it == expressionsList->end()) throw SyntaxException("Expected another symbol",codeReference);
            tmp = *it;
            hashCode = typeid (*tmp).hash_code();
            if(
                hashCode == typeid (OpenParenthesisExpression).hash_code() ||
                hashCode == typeid (MemberAccessExpression).hash_code())
            {
                _value = new ExecutionLinkedValue(expressionsList);
            }
            else if(
                hashCode == typeid (AdditionExpression).hash_code() ||
                hashCode == typeid (SubstractionExpression).hash_code() ||
                hashCode == typeid (DivitionExpression).hash_code() ||
                hashCode == typeid (MultiplicationExpression).hash_code()
                )
            {
                _value = new MathOperationLinkedValue(expressionsList);
            }
            else if(
                hashCode == typeid (EqualExpression).hash_code() ||
                hashCode == typeid (GreaterThanExpression).hash_code() ||
                hashCode == typeid (LessThanExpression).hash_code() ||
                hashCode == typeid (NegationExpression).hash_code()
                )
            {
                _value = new BooleanOperationLinkedValue(expressionsList);
            }
            else
            {
                _value = new NameLinkedValue((NameExpression*)expressionsList->front());
            }
        }
        else if(hashCode == typeid (NumericValueExpression).hash_code())
        {
            auto it = expressionsList->begin();
            advance(it,1);
            if(it == expressionsList->end()) throw SyntaxException("Expected another symbol",codeReference);
            tmp = *it;
            if(
                typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code() ||
                typeid (*tmp).hash_code() == typeid (GreaterThanExpression).hash_code() ||
                typeid (*tmp).hash_code() == typeid (LessThanExpression).hash_code() ||
                typeid (*tmp).hash_code() == typeid (NegationExpression).hash_code()
                )
            {
                _value = new BooleanOperationLinkedValue(expressionsList);
            }
            else
            {
                _value = new MathOperationLinkedValue(expressionsList);
            }
        }
        else
        {
            throw SyntaxException("Expected a valid value",codeReference);
        }
    } catch (SyntaxException& e) {
        throw e;
    } catch (SemanticException& e) {
        throw e;
    } catch (exception& e) {
        throw e;
    }
}

Value* ValueExpression::getValue() const{
    return _value;
}

ValueExpression::~ValueExpression(){}

void ValueExpression::interpret(){}

/// Math operation expression

MathOperationExpression::MathOperationExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){}

MathOperationExpression::~MathOperationExpression(){}

void MathOperationExpression::interpret(){}
