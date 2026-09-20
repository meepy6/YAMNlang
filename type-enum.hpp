#ifndef TYPE_ENUM_H
#define TYPE_ENUM_H

#include <string>
#include <unordered_map>
#include <ostream>

enum class Type {
    // the absence of a meaningful type
    NONE,

    // literals and identifiers
    NUMBER_LITERAL,
    IDENTIFIER,

    // symbols
    OPEN_BRACKET,
    CLOSE_BRACKET,

    ADD,
    ADD_ASSIGN, // new
    INCREMENT, // new

    SUBTRACT,
    NEGATE,
    SUBTRACT_ASSIGN, // new
    DECREMENT, // new

    MULTIPLY,
    MULTIPLY_ASSIGN, // new

    DIVIDE,
    DIVIDE_ASSIGN, // new

    FLOOR_DIVIDE, // new
    FLOOR_DIVIDE_ASSIGN, // new

    MODULO,
    MODULO_ASSIGN, // new

    ASSIGN,

    EQUALITY_COMPARISON, // new
    LESS_THAN_COMPARISON, // new
    MORE_THAN_COMPARISON, // new
    LESS_THAN_EQUALS_COMPARISON, // new
    MORE_THAN_EQUALS_COMPARISON, // new
    INEQUALITY_COMPARISON, // new
    BOOLEAN_INVERSION, // new

    // keywords (denoted by the suffix _KW)
    VAR_KW,
    SHOW_KW,
    IF_KW, // new
    ELSE_KW, // new
    REPEAT_KW, // new
    WHILE_KW, // new
    LOGICAL_AND_KW, // new
    LOGICAL_OR_KW, // new

    // invalid
    INVALID,

    // ambiguous
    AMBIGUOUS,

    // end of expression
    END_OF_EXPRESSION
};

// these values/functions are explained in the source file: type-enum.cpp
extern std::unordered_map<std::string, Type> strings_to_types;
Type findContextOfMinusSymbolBasedOnPreviousToken(const Type& prev);
bool validIdentifier(const std::string& str);
bool validNumberLiteral(const std::string& str);
Type getTokenTypeOfString(const std::string& str, const Type& prev);

std::ostream& operator <<(std::ostream& os, const Type& type);

#endif // TYPE_ENUM_H
