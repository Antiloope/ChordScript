#ifndef LANGUAGECONSTANTS_H
#define LANGUAGECONSTANTS_H

#include <string>

namespace CS {

namespace {
    constexpr size_t getHash(const char* str, int h = 0)
    {
        return !str[h] ? 5381 : (getHash(str, h+1) * 33) ^ str[h];
    }
}

namespace Constants {
    const char EQUAL                    = '=';
    const char SUBSTRACTION             = '-';
    const char ADDITION                 = '+';
    const char MULTIPLICATION           = '*';
    const char DIVITION                 = '/';
    const char NEGATION                 = '!';
    const char GREATER_THAN             = '>';
    const char LESS_THAN                = '<';
    const char OR                       = '|';
    const char AND                      = '&';
    const char MEMBER_ACCESS            = '.';
    const char OPEN_PARENTHESIS         = '(';
    const char CLOSE_PARENTHESIS        = ')';
    const char OPEN_BRACKET             = '[';
    const char CLOSE_BRACKET            = ']';
    const char OPEN_BRACE               = '{';
    const char CLOSE_BRACE              = '}';
    const char END_OF_EXPRESSION        = ';';
    const char SEPARATOR                = ',';
    const char STIRNG_DELIMITATOR       = '"';
    const char START_COMMENT            = '#';
    const char END_OF_LINE              = 0x0A;
    const char HORIZONTAL_TAB           = 0x09;
    const char SPACE                    = ' ';
    const std::string S_NULL            = "null";
    const std::string SAMPLE            = "sample";
    const std::string SOUND             = "sound";
    const std::string NUMBER            = "number";
    const std::string ARGUMENT          = "argument";
    const std::string ARRAY             = "array";
    const std::string GROUP             = "group";
    const std::string BOOLEAN           = "boolean";
    const std::string STRING            = "string";
    const std::string FOR               = "for";
    const std::string IF                = "if";
    const std::string BREAK             = "break";
    const std::string RETURN            = "return";
    const std::string ELSE              = "else";
    const std::string TRUE              = "true";
    const std::string FALSE             = "false";

    /**
     * @brief Check if str is different from all reserved keywords
     * @param str string value to evaluate
     * @return true if str is different from all reserved keywords
     */
    inline bool isValidName(const std::string str)
    {
        switch (getHash(str.c_str())) {
        case getHash("null"):
        case getHash("sample"):
        case getHash("sound"):
        case getHash("number"):
        case getHash("argument"):
        case getHash("array"):
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
