#ifndef LANGUAGECONSTANTS_H
#define LANGUAGECONSTANTS_H

#include <string>

using namespace std;

namespace CS {

namespace  {
    constexpr size_t getHash(const char* str, int h = 0)
    {
        return !str[h] ? 5381 : (getHash(str, h+1) * 33) ^ str[h];
    }
}

namespace constants {
    const char cEqual               = '=';
    const char cSubstraction        = '-';
    const char cAddition            = '+';
    const char cMultiplication      = '*';
    const char cDivition            = '/';
    const char cNegation            = '!';
    const char cGreaterThan         = '>';
    const char cLessThan            = '<';
    const char cOr                  = '|';
    const char cAnd                 = '&';
    const char cMemberAccess        = '.';
    const char cOpenParenthesis     = '(';
    const char cCloseParenthesis    = ')';
    const char cOpenBracket         = '[';
    const char cCloseBracket        = ']';
    const char cOpenBrace           = '{';
    const char cCloseBrace          = '}';
    const char cEndOfExpression     = ';';
    const char cSeparator           = ',';
    const char cStringDelimitator   = '"';
    const char cStartComment        = '#';
    const char cEndOfLine           = 0x0A;
    const char cSpace               = ' ';
    const string sNull              = "null";
    const string sSample            = "sample";
    const string sSound             = "sound";
    const string sNumber            = "number";
    const string sArgument          = "argument";
    const string sGroup             = "group";
    const string sBoolean           = "boolean";
    const string sString            = "string";
    const string sFor               = "for";
    const string sIf                = "if";
    const string sBreak             = "break";
    const string sReturn            = "return";
    const string sElse              = "else";
    const string sTrue              = "true";
    const string sFalse             = "false";

    /**
     * @brief Check if str is different from all reserved keywords
     * @param str string value to evaluate
     * @return true if str is different from all reserved keywords
     */
    inline bool isValidName(const string str)
    {
        switch (getHash(str.c_str())) {
        case getHash("null"):
        case getHash("sample"):
        case getHash("sound"):
        case getHash("number"):
        case getHash("argument"):
        case getHash("group"):
        case getHash("boolean"):
        case getHash("string"):
        case getHash("for"):
        case getHash("if"):
        case getHash("break"):
        case getHash("return"):
        case getHash("else"):
        case getHash("true"):
        case getHash("false"):
            return false;
            break;
        }
        return true;
    }
}

}


#endif // LANGUAGECONSTANTS_H
