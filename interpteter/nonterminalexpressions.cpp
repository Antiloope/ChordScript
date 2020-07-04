#include "nonterminalexpressions.h"
#include "utils/Exceptions/exception.h"
#include "context.h"

using namespace CS;
using namespace std;

LinkedValue* generateLinkedValue(list<TerminalExpression*>*);

////////////////////////////////////////
///     NonTerminalExpression
////////////////////////////////////////

NonTerminalExpression::NonTerminalExpression() : Expression() {}

NonTerminalExpression::NonTerminalExpression(size_t codeReference) : Expression(codeReference) {}

NonTerminalExpression::~NonTerminalExpression(){}

////////////////////////////////////////
///     ProgramExpression
////////////////////////////////////////

ProgramExpression::ProgramExpression(list<TerminalExpression*> expressionsList) : NonTerminalExpression(0){
    while (!expressionsList.empty())
    {
        _instructionsList.push_back(new InstructionExpression(&expressionsList,expressionsList.front()->getCodeReference()));
    }
}

ProgramExpression::ProgramExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
    TerminalExpression* tmp = expressionsList->front();
    while (
        tmp->getType() != cCast(ExpressionTypes::Return) &&
        tmp->getType() != cCast(ExpressionTypes::CloseBrace) &&
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

/// TODO: Implement
void ProgramExpression::interpret(){

}

////////////////////////////////////////
///     InstructionExpression
////////////////////////////////////////

InstructionExpression::InstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
    if(expressionsList->empty()) throw SyntaxException("Expected another instruction",codeReference);

    TerminalExpression* tmp = expressionsList->front();

    switch (tmp->getType())
    {
    case cCast(ExpressionTypes::For):
        _instruction = new ForInstructionExpression(expressionsList,tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::If):
        _instruction = new IfInstructionExpression(expressionsList,tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::Break):
        _instruction = new BreakInstructionExpression(expressionsList,tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::EOE):
        _instruction = new EOEInstructionExpression(expressionsList,tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::Name):
    {
        NameExpression* nameExpression = (NameExpression*)tmp;
        if ( Context::getInstance()->isDataType(nameExpression->getName()) )
        {
            auto it = expressionsList->begin();
            advance(it,1);
            if(it == expressionsList->end()) throw SyntaxException("Expected variable name",codeReference);
            tmp = *it;
            if (
                tmp->getType() == cCast(ExpressionTypes::Name) &&
                Context::getInstance()->isValidName(((NameExpression*)tmp)->getName()))
            {
                advance(it,1);
                if( it == expressionsList->end()) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
                tmp = *it;
                if( tmp->getType() == cCast(ExpressionTypes::Equal) )
                {
                    _instruction = new AssignationExpression(expressionsList,tmp->getCodeReference());
                }
                else if(
                    tmp->getType() == cCast(ExpressionTypes::EOE) ||
                    tmp->getType() == cCast(ExpressionTypes::OpenParenthesis))
                {
                    _instruction = new DefinitionExpression(expressionsList,tmp->getCodeReference());
                }
                else
                {
                    throw SyntaxException("Expected another symbol",tmp->getCodeReference());
                }
            }
            else
            {
                throw SemanticException("Invalid name",tmp->getCodeReference());
            }
        }
        else if( Context::getInstance()->nameExist(nameExpression->getName()) )
        {
            auto it = expressionsList->begin();
            advance(it,1);
            if(it == expressionsList->end()) throw SyntaxException("Expected variable name",codeReference);
            tmp = *it;
            if( tmp->getType() == cCast(ExpressionTypes::Equal) )
            {
                _instruction = new AssignationExpression(expressionsList,tmp->getCodeReference());
            }
            else if(
                tmp->getType() == cCast(ExpressionTypes::MemberAccess) ||
                tmp->getType() == cCast(ExpressionTypes::OpenParenthesis) )
            {
                _instruction = new ExecutionExpression(expressionsList,tmp->getCodeReference());
            }
            else
            {
                throw SyntaxException("Expected another symbol",codeReference);
            }
        }
        else
        {
            throw SyntaxException("Unrecognized variable name or data type",codeReference);
        }
        break;
    }
    default:
        throw SyntaxException("Expected another instruction",codeReference);
    }
}

InstructionExpression::~InstructionExpression(){
    delete _instruction;
}

/// TODO: Implement
void InstructionExpression::interpret(){

}

////////////////////////////////////////
///     ForInstructionExpression
////////////////////////////////////////

/// TODO: Implement
ForInstructionExpression::ForInstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
}

ForInstructionExpression::~ForInstructionExpression(){

}

/// TODO: Implement
void ForInstructionExpression::interpret(){

}

////////////////////////////////////////
///     IfInstructionExpression
////////////////////////////////////////

/// TODO: Implement
IfInstructionExpression::IfInstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
}

IfInstructionExpression::~IfInstructionExpression(){

}

/// TODO: Implement
void IfInstructionExpression::interpret(){

}

////////////////////////////////////////
///     BreakInstructionExpression
////////////////////////////////////////

/// TODO: Implement
BreakInstructionExpression::BreakInstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
}

BreakInstructionExpression::~BreakInstructionExpression(){}

/// TODO: Implement
void BreakInstructionExpression::interpret(){

}

////////////////////////////////////////
///     EOEInstructionExpression
////////////////////////////////////////

EOEInstructionExpression::EOEInstructionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
    expressionsList->pop_front();
}

EOEInstructionExpression::~EOEInstructionExpression(){}

/// TODO: Implement
void EOEInstructionExpression::interpret(){}

////////////////////////////////////////
///     AssignationExpression
////////////////////////////////////////

AssignationExpression::AssignationExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
    TerminalExpression* tmp = expressionsList->front();
    if(
        tmp->getType() == cCast(ExpressionTypes::Name) &&
        Context::getInstance()->isDataType(((NameExpression*)tmp)->getName()) )
    {
        _dataType = ((NameExpression*)tmp)->getName();
        expressionsList->pop_front();
        if( expressionsList->empty()) throw SyntaxException("Expected variable name",tmp->getCodeReference() );
        tmp = expressionsList->front();
        if( tmp->getType() == cCast(ExpressionTypes::Name) )
        {
            _varName = ((NameExpression*)tmp)->getName();
            expressionsList->pop_front();
            if( expressionsList->empty()) throw SyntaxException("Expected = after name",tmp->getCodeReference() );
            tmp = expressionsList->front();
            if( tmp->getType() == cCast(ExpressionTypes::Equal) )
            {
                expressionsList->pop_front();
                if( expressionsList->empty()) throw SyntaxException("Expected a value after = ",tmp->getCodeReference() );
                tmp = expressionsList->front();

                Context::getInstance()->newVariable( _varName, _dataType, generateLinkedValue(expressionsList) );
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
    else if( Context::getInstance()->nameExist(((NameExpression*)tmp)->getName()) )
    {
        _varName = ((NameExpression*)tmp)->getName();
        _dataType = Context::getInstance()->getDataTypeName(_varName);
        expressionsList->pop_front();
        if( expressionsList->empty()) throw SyntaxException("Expected = after name",tmp->getCodeReference() );
        tmp = expressionsList->front();
        if( tmp->getType() == cCast(ExpressionTypes::Equal) )
        {
            expressionsList->pop_front();
            if( expressionsList->empty()) throw SyntaxException("Expected a value after = ",tmp->getCodeReference() );
            tmp = expressionsList->front();

            Context::getInstance()->newVariable( _varName, _dataType,generateLinkedValue(expressionsList) );
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
}

/// TODO: Implement
void AssignationExpression::interpret(){}

////////////////////////////////////////
///     DefinitionExpression
////////////////////////////////////////

#include "functiondefinition.h"

DefinitionExpression::DefinitionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){
    TerminalExpression* tmp = expressionsList->front();

    if(
        tmp->getType() != cCast(ExpressionTypes::Name) ||
        !Context::getInstance()->isDataType(((NameExpression*)tmp)->getName()) )
    {
        throw SyntaxException("Expected data type",tmp->getCodeReference());
    }

    string dataType = ((NameExpression*)tmp)->getName();

    expressionsList->pop_front();
    if( expressionsList->empty()) throw SyntaxException("Expected a name",tmp->getCodeReference() );
    tmp = expressionsList->front();

    if( tmp->getType() != cCast(ExpressionTypes::Name) ) throw SyntaxException("Expected a name",tmp->getCodeReference());

    string name = ((NameExpression*)tmp)->getName();

    expressionsList->pop_front();
    if( expressionsList->empty()) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
    tmp = expressionsList->front();

    if( tmp->getType() == cCast(ExpressionTypes::EOE) )
    {
        expressionsList->pop_front();
        Context::getInstance()->newVariable(name,dataType,nullptr);
    }
    else if ( tmp->getType() == cCast(ExpressionTypes::OpenParenthesis) )
    {
        FunctionDefinition* function = new FunctionDefinition(expressionsList);
        Context::getInstance()->newFunction(name,dataType,function);
    }
    else
    {
        throw SyntaxException("Expected an end of line or a function definition",tmp->getCodeReference());
    }
}

DefinitionExpression::~DefinitionExpression(){}

/// TODO: Implement
void DefinitionExpression::interpret(){}

////////////////////////////////////////
///     ExecutionExpression
////////////////////////////////////////

/// TODO: Implement
ExecutionExpression::ExecutionExpression(list<TerminalExpression*>* expressionsList, size_t codeReference) : NonTerminalExpression(codeReference){}

ExecutionExpression::~ExecutionExpression(){}

/// TODO: Implement
void ExecutionExpression::interpret(){}

////////////////////////////////////////
///     generateLinkedValue
////////////////////////////////////////

LinkedValue* generateLinkedValue(list<TerminalExpression*>* terminalExpressionsList){
    TerminalExpression* tmp = terminalExpressionsList->front();

    LinkedValue* ret;

    switch ( tmp->getType() )
    {
    case cCast(ExpressionTypes::String):
        ret = new StringLinkedValue((StringExpression*)tmp); // TODO: Correct to use simply a string instead of StringExpression
        break;
    case cCast(ExpressionTypes::Null):
        ret = new NullLinkedValue();
        break;
    case cCast(ExpressionTypes::Addition):
    case cCast(ExpressionTypes::Substract):
        ret = new MathOperationLinkedValue(terminalExpressionsList);
        break;
    case cCast(ExpressionTypes::OpenBracket):
    case cCast(ExpressionTypes::OpenParenthesis):
        ret = new BooleanOperationLinkedValue(terminalExpressionsList);
        break;
    case cCast(ExpressionTypes::Name):
    {
        auto it = terminalExpressionsList->begin();
        advance(it,1);
        if( it == terminalExpressionsList->end() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
        tmp = *it;
        switch (tmp->getType()) {
        case cCast(ExpressionTypes::OpenParenthesis):
        case cCast(ExpressionTypes::MemberAccess):
            ret = new ExecutionLinkedValue(terminalExpressionsList);
            break;
        case cCast(ExpressionTypes::Addition):
        case cCast(ExpressionTypes::Substract):
        case cCast(ExpressionTypes::Divition):
        case cCast(ExpressionTypes::Multiplication):
            ret = new MathOperationLinkedValue(terminalExpressionsList);
            break;
        case cCast(ExpressionTypes::Equal):
        case cCast(ExpressionTypes::GreaterThan):
        case cCast(ExpressionTypes::LessThan):
        case cCast(ExpressionTypes::Negation):
            ret = new BooleanOperationLinkedValue(terminalExpressionsList);
            break;
        default:
            ret = new NameLinkedValue((NameExpression*)terminalExpressionsList->front());
            break;
        }
        break;
    }
    case cCast(ExpressionTypes::Numeric):
    {
        auto it = terminalExpressionsList->begin();
        advance(it,1);
        if(it == terminalExpressionsList->end()) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
        tmp = *it;
        if(
            tmp->getType() == cCast(ExpressionTypes::Equal) ||
            tmp->getType() == cCast(ExpressionTypes::GreaterThan) ||
            tmp->getType() == cCast(ExpressionTypes::LessThan)||
            tmp->getType() == cCast(ExpressionTypes::Negation)
            )
        {
            ret = new BooleanOperationLinkedValue(terminalExpressionsList);
        }
        else
        {
            ret = new MathOperationLinkedValue(terminalExpressionsList);
        }
        break;
    }
    default:
        throw SyntaxException("Expected a valid value",tmp->getCodeReference());
        break;
    }
    return ret;
}
