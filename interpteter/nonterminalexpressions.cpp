#include "nonterminalexpressions.h"
#include "utils/Exceptions/exception.h"
#include "context.h"
#include <memory>

using namespace CS;
using namespace std;

////////////////////////////////////////
///     NonTerminalExpression
////////////////////////////////////////

NonTerminalExpression::NonTerminalExpression() : Expression() {}

NonTerminalExpression::NonTerminalExpression(size_t codeReference) : Expression(codeReference) {}

NonTerminalExpression::~NonTerminalExpression(){}

////////////////////////////////////////
///     ProgramExpression
////////////////////////////////////////

ProgramExpression::ProgramExpression() : NonTerminalExpression(0) {}

ProgramExpression::ProgramExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

void ProgramExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();
    while (
        !terminalExpressionsList->empty() &&
        tmp->getType() != cCast(ExpressionTypes::CloseBrace) )
    {
        InstructionExpression* instruction = new InstructionExpression(tmp->getCodeReference());
        _instructionsList.push_back(instruction);
        instruction->load(terminalExpressionsList);
        tmp = terminalExpressionsList->front();
    }
}

void ProgramExpression::interpret(list<TerminalExpression*> terminalExpressionsList) {
    while (!terminalExpressionsList.empty())
    {
        InstructionExpression tmp(terminalExpressionsList.front()->getCodeReference());
        tmp.load(&terminalExpressionsList);
        tmp.interpret();
    }
}

void ProgramExpression::interpret(){
    Context::getInstance()->setReturnValue(nullptr);
    for( InstructionExpression* instruction : _instructionsList )
    {
        instruction->interpret();
        if( Context::getInstance()->existReturnValue() )
            break;
    }
}

ProgramExpression::~ProgramExpression() {
    while( !_instructionsList.empty() )
    {
        InstructionExpression* tmp = _instructionsList.front();
        delete tmp;
        _instructionsList.pop_front();
    }
}

////////////////////////////////////////
///     InstructionExpression
////////////////////////////////////////

InstructionExpression::InstructionExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

InstructionExpression::InstructionExpression(const InstructionExpression& e) : NonTerminalExpression(e.getCodeReference()) {
    _instruction = e._instruction;
}

void InstructionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    switch (tmp->getType())
    {
    case cCast(ExpressionTypes::For):
        _instruction = new ForInstructionExpression(tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::If):
        _instruction = new IfInstructionExpression(tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::Break):
        _instruction = new BreakInstructionExpression(tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::EOE):
        _instruction = new EOEInstructionExpression(tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::Return):
        _instruction = new ReturnInstructionExpression(tmp->getCodeReference());
        break;
    case cCast(ExpressionTypes::Name):
    {
        NameExpression* nameExpression = (NameExpression*)tmp;
        if ( Context::getInstance()->isDataType(DataType::getDataTypeId(nameExpression->getName())) )
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if(it == terminalExpressionsList->end()) throw SyntaxException("Expected variable name",tmp->getCodeReference());
            tmp = *it;
            if (
                tmp->getType() == cCast(ExpressionTypes::Name) &&
                Context::getInstance()->isValidName(((NameExpression*)tmp)->getName()))
            {
                advance(it,1);
                if( it == terminalExpressionsList->end()) throw SyntaxException("Expected another symbol",tmp->getCodeReference());
                tmp = *it;
                if( tmp->getType() == cCast(ExpressionTypes::Equal) )
                {
                    _instruction = new AssignationExpression(tmp->getCodeReference());
                }
                else if(
                    tmp->getType() == cCast(ExpressionTypes::EOE) ||
                    tmp->getType() == cCast(ExpressionTypes::OpenParenthesis))
                {
                    _instruction = new DefinitionExpression(tmp->getCodeReference());
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
        else if( Context::getInstance()->isValidName(nameExpression->getName()) )
        {
            auto it = terminalExpressionsList->begin();
            advance(it,1);
            if(it == terminalExpressionsList->end()) throw SyntaxException("Expected variable name",tmp->getCodeReference());
            tmp = *it;
            if( tmp->getType() == cCast(ExpressionTypes::Equal) )
            {
                _instruction = new AssignationExpression(tmp->getCodeReference());
            }
            else if(
                tmp->getType() == cCast(ExpressionTypes::MemberAccess) ||
                tmp->getType() == cCast(ExpressionTypes::OpenParenthesis) )
            {
                _instruction = new ExecutionExpression(tmp->getCodeReference());
            }
            else
            {
                throw SyntaxException("Expected another symbol",tmp->getCodeReference());
            }
        }
        else
        {
            throw SyntaxException("Unrecognized variable name or data type",tmp->getCodeReference());
        }
        break;
    }
    default:
        throw SyntaxException("Expected another instruction",tmp->getCodeReference());
    }

    _instruction->load(terminalExpressionsList);
}

InstructionExpression::~InstructionExpression(){
    if( _instruction ) delete _instruction;
}

void InstructionExpression::interpret(){
    _instruction->interpret();
}

////////////////////////////////////////
///     ForInstructionExpression
////////////////////////////////////////

ForInstructionExpression::ForInstructionExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

void ForInstructionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();
    if( tmp->getType() != cCast(ExpressionTypes::For) ) throw SyntaxException("Expected for",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty()) throw SyntaxException("Expected for arguments",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    if( tmp->getType() != cCast(ExpressionTypes::OpenParenthesis) ) throw SyntaxException("Expected (",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty()) throw SyntaxException("Expected for arguments",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    Context* ctx = Context::getInstance();

    _context = ctx->newScope();

    _assignation = new AssignationExpression(tmp->getCodeReference());
    _assignation->load(terminalExpressionsList);

    if( terminalExpressionsList->empty() || terminalExpressionsList->front()->getType() != cCast(ExpressionTypes::EOE) ) throw SyntaxException("Expected ;",tmp->getCodeReference() );
    terminalExpressionsList->pop_front();
    tmp = terminalExpressionsList->front();

    _booleanOperation = new BooleanOperationLinkedValue(tmp->getCodeReference());
    _booleanOperation->load(terminalExpressionsList);

    if( terminalExpressionsList->empty() || terminalExpressionsList->front()->getType() != cCast(ExpressionTypes::EOE) ) throw SyntaxException("Expected ;",tmp->getCodeReference() );
    terminalExpressionsList->pop_front();
    tmp = terminalExpressionsList->front();

    _endAssignation = new AssignationExpression(tmp->getCodeReference());
    _endAssignation->load(terminalExpressionsList);

    if( terminalExpressionsList->empty() || terminalExpressionsList->front()->getType() != cCast(ExpressionTypes::CloseParenthesis) ) throw SyntaxException("Expected )",tmp->getCodeReference() );
    terminalExpressionsList->pop_front();
    tmp = terminalExpressionsList->front();

    if( terminalExpressionsList->empty() || terminalExpressionsList->front()->getType() != cCast(ExpressionTypes::OpenBrace) ) throw SyntaxException("Expected {",tmp->getCodeReference() );
    terminalExpressionsList->pop_front();
    tmp = terminalExpressionsList->front();

    _function = new ProgramExpression(tmp->getCodeReference());
    _function->load(terminalExpressionsList);

    if( terminalExpressionsList->empty() ) throw SyntaxException("Expected }",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();
    if( tmp->getType() != cCast(ExpressionTypes::CloseBrace) ) throw SyntaxException("Expected }",tmp->getCodeReference());

    terminalExpressionsList->pop_front();

    ctx->returnScope();
}

ForInstructionExpression::~ForInstructionExpression(){
    if( _function ) delete _function;
    if( _endAssignation ) delete _endAssignation;
    if( _assignation ) delete _assignation;
    if( _booleanOperation ) delete _booleanOperation;

    Context::getInstance()->removeScope(_runningContext);
    Context::getInstance()->removeScope(_context);
}

void ForInstructionExpression::interpret(){
    _runningContext = Context::getInstance()->switchScope(_context);
    scope_index tmp = _runningContext;
    if( !_runningContext )
        throw SemanticException("Invalid switch to unknown context",this->getCodeReference());

    Context::getInstance()->setReturnValue(nullptr);
    _assignation->interpret();
    while(
        !Context::getInstance()->existReturnValue() &&
        *(bool*)_booleanOperation->getValue()->getValue() )
    {
        _function->interpret();
        _endAssignation->interpret();
    }
    Context::getInstance()->removeScope(tmp);
}

////////////////////////////////////////
///     IfInstructionExpression
////////////////////////////////////////

IfInstructionExpression::IfInstructionExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

void IfInstructionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();
    if( tmp->getType() != cCast(ExpressionTypes::If) ) throw SyntaxException("Expected if",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty()) throw SyntaxException("Expected if arguments",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    if( tmp->getType() != cCast(ExpressionTypes::OpenParenthesis) ) throw SyntaxException("Expected (",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty()) throw SyntaxException("Expected if arguments",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    _condition = new BooleanOperationLinkedValue(tmp->getCodeReference());
    _condition->load(terminalExpressionsList);

    if( terminalExpressionsList->empty()) throw SyntaxException("Expected )",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();
    if( tmp->getType() != cCast(ExpressionTypes::CloseParenthesis) ) throw SyntaxException("Expected )",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty()) throw SyntaxException("Expected {",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();
    if( tmp->getType() != cCast(ExpressionTypes::OpenBrace) ) throw SyntaxException("Expected {",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty()) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    Context* ctx = Context::getInstance();

    _context = ctx->newScope();

    _function = new ProgramExpression(tmp->getCodeReference());
    _function->load(terminalExpressionsList);

    ctx->returnScope();

    if( terminalExpressionsList->empty() ) throw SyntaxException("Expected }",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();
    if( tmp->getType() != cCast(ExpressionTypes::CloseBrace) ) throw SyntaxException("Expected }",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty() || terminalExpressionsList->front()->getType() != cCast(ExpressionTypes::Else) )
    {
        _elseContext = 0;
        _elseFunction = nullptr;
    }
    else
    {
        terminalExpressionsList->pop_front();
        if( terminalExpressionsList->empty() ) throw SyntaxException("Expected {",tmp->getCodeReference() );
        tmp = terminalExpressionsList->front();

        if( tmp->getType() != cCast(ExpressionTypes::OpenBrace) ) throw SyntaxException("Expected {",tmp->getCodeReference());

        terminalExpressionsList->pop_front();
        if( terminalExpressionsList->empty()) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
        tmp = terminalExpressionsList->front();

        _elseContext = ctx->newScope();

        _elseFunction = new ProgramExpression(tmp->getCodeReference());
        _elseFunction->load(terminalExpressionsList);

        ctx->returnScope();

        if( terminalExpressionsList->empty() ) throw SyntaxException("Expected }",tmp->getCodeReference() );
        tmp = terminalExpressionsList->front();
        if( tmp->getType() != cCast(ExpressionTypes::CloseBrace) ) throw SyntaxException("Expected }",tmp->getCodeReference());
        terminalExpressionsList->pop_front();
    }
}

IfInstructionExpression::~IfInstructionExpression(){
    if( _function ) delete _function;
    if( _elseFunction ) delete _elseFunction;
    if( _condition ) delete _condition;

    if( _runningContext ) Context::getInstance()->removeScope(_runningContext);
    if( _runningElseContext ) Context::getInstance()->removeScope(_runningElseContext);
    if( _context ) Context::getInstance()->removeScope(_context);
    if( _elseContext ) Context::getInstance()->removeScope(_elseContext);
}

void IfInstructionExpression::interpret(){
    Context::getInstance()->setReturnValue(nullptr);
    if( *(bool*)_condition->getValue()->getValue() )
    {
        _runningContext = Context::getInstance()->switchScope(_context);
        scope_index tmp = _runningContext;
        if( !_runningContext )
            throw SemanticException("Invalid switch to unknown context",this->getCodeReference());

        _function->interpret();

        Context::getInstance()->removeScope(tmp);
    }
    else
    {
        if( _elseFunction )
        {
            _runningElseContext = Context::getInstance()->switchScope(_elseContext);
            scope_index tmp = _runningElseContext;
            if( !_runningElseContext )
                throw SemanticException("Invalid switch to unknown context",this->getCodeReference());

            _elseFunction->interpret();

            Context::getInstance()->removeScope(tmp);
        }
    }
}

////////////////////////////////////////
///     BreakInstructionExpression
////////////////////////////////////////

BreakInstructionExpression::BreakInstructionExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

void BreakInstructionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    if(Context::getInstance()->getCurrentScope() == GlobalScope)
        throw SyntaxException("Break is not valid in global scope",terminalExpressionsList->front()->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty() ||
        terminalExpressionsList->front()->getType() != cCast(ExpressionTypes::EOE)
        )
        throw SyntaxException("Expected ;",terminalExpressionsList->front()->getCodeReference());
}

BreakInstructionExpression::~BreakInstructionExpression() {}

void BreakInstructionExpression::interpret() {
    Context::getInstance()->setReturnValue();
}

////////////////////////////////////////
///     EOEInstructionExpression
////////////////////////////////////////

EOEInstructionExpression::EOEInstructionExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

void EOEInstructionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    terminalExpressionsList->pop_front();
}

EOEInstructionExpression::~EOEInstructionExpression() {}

void EOEInstructionExpression::interpret() {}

////////////////////////////////////////
///     AssignationExpression
////////////////////////////////////////

AssignationExpression::AssignationExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

void AssignationExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();
    DataTypesId dataType;
    bool isDataTypeDefinition = false;
    if(
        tmp->getType() == cCast(ExpressionTypes::Name) &&
        Context::getInstance()->isDataType(DataType::getDataTypeId(((NameExpression*)tmp)->getName())) )
    {
        dataType = DataType::getDataTypeId(((NameExpression*)tmp)->getName());

        terminalExpressionsList->pop_front();
        if( terminalExpressionsList->empty()) throw SyntaxException("Expected variable name",tmp->getCodeReference() );
        tmp = terminalExpressionsList->front();

        if( tmp->getType() != cCast(ExpressionTypes::Name) ) throw SyntaxException("Expected variable name",tmp->getCodeReference());
        isDataTypeDefinition = true;
    }
    else if( Context::getInstance()->nameExist(((NameExpression*)tmp)->getName()) )
    {
        dataType = Context::getInstance()->getVariableDataType(((NameExpression*)tmp)->getName());
    }
    else
    {
        throw SyntaxException("Expected another symbol",tmp->getCodeReference());
    }

    _varName = ((NameExpression*)tmp)->getName();

    if( !Context::getInstance()->isValidName(_varName) ) throw SyntaxException("Invalid name",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty()) throw SyntaxException("Expected = after name",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    if( tmp->getType() != cCast(ExpressionTypes::Equal) ) throw SyntaxException("Expected = ",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty()) throw SyntaxException("Expected a value after = ",tmp->getCodeReference() );
    terminalExpressionsList->front();

    _value = LinkedValue::generateLinkedValue(terminalExpressionsList);
    if (
        terminalExpressionsList->front()->getType() != cCast(ExpressionTypes::EOE) &&
        terminalExpressionsList->front()->getType() != cCast(ExpressionTypes::CloseParenthesis) )
        throw SyntaxException("Expected ;",terminalExpressionsList->front()->getCodeReference());

    if( isDataTypeDefinition )
    {
        Context::getInstance()->newVariable( _varName, dataType);
    }
}

AssignationExpression::~AssignationExpression() {
    if( _value ) delete _value;
}

void AssignationExpression::interpret() {
    if( !Context::getInstance()->setVariableValue(_varName,_value->getValue()) )
    {
        string errorDescription = "Invalid conversion from ";
        errorDescription.append(DataType::getDataTypeString(_value->getDataTypeId()));
        errorDescription.append(" to ");
        errorDescription.append(DataType::getDataTypeString(Context::getInstance()->getVariableDataType(_varName)));
        throw SemanticException(errorDescription,this->getCodeReference());
    }
}

////////////////////////////////////////
///     DefinitionExpression
////////////////////////////////////////

#include "functiondefinition.h"

DefinitionExpression::DefinitionExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

void DefinitionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    if(
        tmp->getType() != cCast(ExpressionTypes::Name) ||
        !Context::getInstance()->isDataType(DataType::getDataTypeId(((NameExpression*)tmp)->getName())) )
    {
        throw SyntaxException("Expected data type",tmp->getCodeReference());
    }

    DataTypesId dataType = DataType::getDataTypeId(((NameExpression*)tmp)->getName());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty()) throw SyntaxException("Expected a name",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    if( tmp->getType() != cCast(ExpressionTypes::Name) ) throw SyntaxException("Expected a name",tmp->getCodeReference());

    if( !Context::getInstance()->isValidName(((NameExpression*)tmp)->getName()) ) throw SyntaxException("Invalid name", tmp->getCodeReference());

    string varName = ((NameExpression*)tmp)->getName();

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty()) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    if( tmp->getType() == cCast(ExpressionTypes::EOE) )
    {
        terminalExpressionsList->pop_front();
        Context::getInstance()->newVariable(varName,dataType);
    }
    else if ( tmp->getType() == cCast(ExpressionTypes::OpenParenthesis) )
    {
        FunctionDefinition* function = Context::getInstance()->newFunction(varName,dataType);
        function->load(terminalExpressionsList);
    }
    else
    {
        throw SyntaxException("Expected an end of line or a function definition",tmp->getCodeReference());
    }
}

DefinitionExpression::~DefinitionExpression() {}

void DefinitionExpression::interpret() {}

////////////////////////////////////////
///     ReturnExpression
////////////////////////////////////////

ReturnInstructionExpression::ReturnInstructionExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

void ReturnInstructionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    if( tmp->getType() != cCast(ExpressionTypes::Return) ) throw SyntaxException("Expected return", tmp->getCodeReference());

    if( Context::getInstance()->getCurrentScope() == GlobalScope ) throw SyntaxException("Return is not valid in global scope",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if ( terminalExpressionsList->empty() ) throw SyntaxException("Expected a return value or end of line",tmp->getCodeReference());
    tmp = terminalExpressionsList->front();

    if ( tmp->getType() == cCast(ExpressionTypes::EOE) )
    {
        _returnValue = new NullLinkedValue(tmp->getCodeReference());
        _returnValue->load(terminalExpressionsList);
    }
    else
    {
        _returnValue = LinkedValue::generateLinkedValue(terminalExpressionsList);
        tmp = terminalExpressionsList->front();
        if ( tmp->getType() != cCast(ExpressionTypes::EOE) ) throw SyntaxException("Expected ;",tmp->getCodeReference());
    }
}

ReturnInstructionExpression::~ReturnInstructionExpression() {
    if( _returnValue ) delete _returnValue;
}

void ReturnInstructionExpression::interpret() {
    auto ret = _returnValue->getValue();
    Context::getInstance()->setReturnValue(ret);
    if( ret ) delete ret;
}

////////////////////////////////////////
///     ExecutionExpression
////////////////////////////////////////

ExecutionExpression::ExecutionExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

void ExecutionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    if ( tmp->getType() != cCast(ExpressionTypes::Name) ) throw SyntaxException("Expected a name", tmp->getCodeReference());

    _name = ((NameExpression*)tmp)->getName();
    if ( !Context::getInstance()->isValidName(_name) ) throw SyntaxException("Invalid name",tmp->getCodeReference());
    if ( !Context::getInstance()->nameExist(_name) && !Context::getInstance()->functionNameExist(_name) ) throw SemanticException("Unrecognized name",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if ( terminalExpressionsList->empty() ) throw SyntaxException("Expected arguments",tmp->getCodeReference());
    tmp = terminalExpressionsList->front();

    if ( tmp->getType() == cCast(ExpressionTypes::OpenParenthesis) )
    {
        ArrayLinkedValue* aux = new ArrayLinkedValue(tmp->getCodeReference());
        _methodsList.push_back(tuple<string,ArrayLinkedValue*>(_name,aux));
        aux->load(terminalExpressionsList);
    }
    bool isValidMethod = true;
    while(isValidMethod)
    {
        if( tmp->getType() == cCast(ExpressionTypes::MemberAccess) )
        {
            terminalExpressionsList->pop_front();
            if ( terminalExpressionsList->empty() ) throw SyntaxException("Expected arguments",tmp->getCodeReference());
            tmp = terminalExpressionsList->front();

            if ( tmp->getType() != cCast(ExpressionTypes::Name) ) throw SyntaxException("Expected a name", tmp->getCodeReference());

            string name = ((NameExpression*)tmp)->getName();

            terminalExpressionsList->pop_front();
            if ( terminalExpressionsList->empty() ) throw SyntaxException("Expected (",tmp->getCodeReference());
            tmp = terminalExpressionsList->front();

            if ( tmp->getType() != cCast(ExpressionTypes::OpenParenthesis) ) throw SyntaxException("Expected (",tmp->getCodeReference());

            ArrayLinkedValue* aux = new ArrayLinkedValue(tmp->getCodeReference());
            _methodsList.push_back(tuple<string,ArrayLinkedValue*>(name,aux));
            aux->load(terminalExpressionsList);
        }
        else
        {
            isValidMethod = false;
            if ( !terminalExpressionsList->front() || terminalExpressionsList->front()->getType() != cCast(ExpressionTypes::EOE) ) throw SyntaxException("Expected ;",tmp->getCodeReference());
        }
    }
}

ExecutionExpression::~ExecutionExpression(){
    while( !_methodsList.empty() )
    {
        tuple<string,ArrayLinkedValue*> tmp = _methodsList.front();
        delete get<1>(tmp);
        _methodsList.pop_front();
    }
}

void ExecutionExpression::interpret() {
    if( _name == get<0>(_methodsList.front()) )
    {
        if( !Context::getInstance()->executeFunction(_name,get<1>(_methodsList.front())->getValue()) )
        {
            throw SemanticException("Unknown function name", this->getCodeReference());
        }
    }
    else
    {
        for( auto method : _methodsList )
        {
            unique_ptr<LiteralValue> args = unique_ptr<LiteralValue>(get<1>(method)->getValue());
            if( !Context::getInstance()->executeMethod(_name,get<0>(method),args.get()) )
            {
                throw SyntaxException("Unknown method name",get<1>(method)->getCodeReference());
            }
        }
    }
}
