#ifndef TYPE_ENUM_H
#define TYPE_ENUM_H

#include <string>
#include <unordered_map>
#include <ostream>
#include <vector>

enum class Type {
    // the absence of a meaningful type
    NONE,

    // literals and identifiers
    NUMBER_LITERAL,
    STRING_LITERAL,
    IDENTIFIER,

    // symbols
    OPEN_BRACKET,
    CLOSE_BRACKET,

    ADD,
    ADD_ASSIGN,
    INCREMENT,

    SUBTRACT,
    NEGATE,
    SUBTRACT_ASSIGN,
    DECREMENT,

    MULTIPLY,
    MULTIPLY_ASSIGN,

    DIVIDE,
    DIVIDE_ASSIGN,

    FLOOR_DIVIDE,
    FLOOR_DIVIDE_ASSIGN,

    MODULO,
    MODULO_ASSIGN,

    ASSIGN,

    EQUALITY_COMPARISON,
    LESS_THAN_COMPARISON,
    MORE_THAN_COMPARISON,
    LESS_THAN_EQUALS_COMPARISON,
    MORE_THAN_EQUALS_COMPARISON,
    INEQUALITY_COMPARISON,
    BOOLEAN_INVERSION,

    // keywords (denoted by the suffix _KW)
    VAR_KW,
    SHOW_KW,
    IF_KW,
    ELSE_KW,
    REPEAT_KW,
    WHILE_KW,
    LOGICAL_AND_KW,
    LOGICAL_OR_KW,

    // invalid
    INVALID,

    // ambiguous
    AMBIGUOUS,

    // end of expression
    END_OF_EXPRESSION
};

// these values/functions are explained in the source file: type-enum.cpp
extern const std::vector<std::string> valid_symbols;
Type findContextOfMinusSymbolBasedOnPreviousToken(const Type& prev);
bool validIdentifier(const std::string& str);
bool validNumberLiteral(const std::string& str);
bool validStringLiteral(const std::string& str);
Type getTokenTypeOfCode(const std::string& str, const Type& prev);
Type getTokenTypeOfSymbol(const std::string& str, const Type& prev);
Type getTokenTypeOfKeyword(const std::string& str);

std::ostream& operator <<(std::ostream& os, const Type& type);

#endif // TYPE_ENUM_H
