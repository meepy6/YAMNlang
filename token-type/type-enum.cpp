#include "type-enum.hpp"
#include <cctype>
using namespace std;

// all valid symbols (computed in language_parse.cpp using a prefix tree)
const vector<string> valid_symbols = {"+", "++", "+=", "-", "--", "-=", "*", "*=", "/", "/=", "//", "//=", "!=", "!", "%", "%=", "=", "==", "<", ">", "<=", ">=", "(", ")"};

// based on the previous token's type, determines what a succeeding '-'
// would mean (Type::SUBTRACT or Type::NEGATE)
Type findContextOfMinusSymbolBasedOnPreviousToken(const Type& prev) {
    switch (prev) {
        // subtract
        case Type::INCREMENT:
        case Type::DECREMENT:
        case Type::CLOSE_BRACKET:
        case Type::STRING_LITERAL:
        case Type::IDENTIFIER:
        case Type::NUMBER_LITERAL: return Type::SUBTRACT;

        // negate
        case Type::SHOW_KW:
        case Type::VAR_KW:
        case Type::IF_KW:
        case Type::ELSE_KW:
        case Type::REPEAT_KW:
        case Type::WHILE_KW:
        case Type::LOGICAL_AND_KW:
        case Type::LOGICAL_OR_KW:

        case Type::NONE: // (shouldn't ever exist anyway, but I don't like compiler warnings LOL)

        case Type::ADD:
        case Type::ADD_ASSIGN:

        case Type::SUBTRACT:
        case Type::SUBTRACT_ASSIGN:

        case Type::MULTIPLY:
        case Type::MULTIPLY_ASSIGN:

        case Type::DIVIDE:
        case Type::DIVIDE_ASSIGN:

        case Type::FLOOR_DIVIDE:
        case Type::FLOOR_DIVIDE_ASSIGN:

        case Type::MODULO:
        case Type::MODULO_ASSIGN:

        case Type::ASSIGN:

        case Type::EQUALITY_COMPARISON:
        case Type::LESS_THAN_COMPARISON:
        case Type::MORE_THAN_COMPARISON:
        case Type::LESS_THAN_EQUALS_COMPARISON:
        case Type::MORE_THAN_EQUALS_COMPARISON:
        case Type::INEQUALITY_COMPARISON:
        case Type::BOOLEAN_INVERSION:

        case Type::NEGATE:

        case Type::OPEN_BRACKET:
        case Type::END_OF_EXPRESSION: return Type::NEGATE;

        // ambiguous (cannot be determined)
        case Type::INVALID:
        case Type::AMBIGUOUS: return Type::AMBIGUOUS; break;
    }
}

// determines if the string str would be a valid identifier
//
// RULES FOR VALID IDENTIFIER: similar to Java, C and other languages,
// first character must be an underscore ('_') or a letter,
// subsequent characters must be underscores ('_'), letters or numeric digits (0-9)
bool validIdentifier(const string& str) {
    if (str == "") return false;

    // isalpha instead of isalnum: identifiers may only start with a letter or underscore
    if (!(isalpha(static_cast<unsigned char>(str.front())) || str.front() == '_')) {
        return false;
    }

    for (size_t i = 1; i < str.length(); ++i) {
        // any character except for first may be alphanumeric,
        // and we have already checked str.front(),
        // so now use isalnum
        if (!(isalnum(static_cast<unsigned char>(str[i])) || str[i] == '_')) {
            return false;
        }
    }

    return true;
}

// determines if the string str would be a valid number literal
// i.e. any whole number (0, 1, 2...) with no decimals allowed (yet!)
bool validNumberLiteral(const string& str) {
    if (str == "") return false;

    for (size_t i = 0; i < str.length(); ++i) {
        // all characters must be digits (no decimals, yet)
        if (!isdigit(static_cast<unsigned char>(str[i]))) {
            return false;
        }
    }

    return true;
}

bool validStringLiteral(const string& str) {
    if (str == "") return false;

    if (str.front() != '"' || str.back() != '"') return false;

    for (size_t i = 0; i < str.length(); ++i) {
        // no character may be " (not supporting escape chars yet)
        if (str[i] == '"') {
            return false;
        }
    }

    return true;
}

// find the type of string str, but only if it is a keyword
// (all valid keywords have _KW suffixed to their Type enum value)
Type getTokenTypeOfKeyword(const string& str) {
    if (str == "var") return Type::VAR_KW;
    else if (str == "show") return Type::SHOW_KW;
    else if (str == "if") return Type::IF_KW;
    else if (str == "else") return Type::ELSE_KW;
    else if (str == "repeat") return Type::REPEAT_KW;
    else if (str == "while") return Type::WHILE_KW;
    else if (str == "and") return Type::LOGICAL_AND_KW;
    else if (str == "or") return Type::LOGICAL_OR_KW;

    else return Type::INVALID; // str may be a valid token, but it is not a valid keyword
}

// finds the type of string str, but only if it is a symbol
// (all valid symbols are defined in valid_symbols above)
Type getTokenTypeOfSymbol(const string& str, const Type& prev) {
    if (str == "(") return Type::OPEN_BRACKET;
    else if (str == ")") return Type::CLOSE_BRACKET;
    else if (str == "+") return Type::ADD;
    else if (str == "++") return Type::INCREMENT;
    else if (str == "+=") return Type::ADD_ASSIGN;
    else if (str == "-") return findContextOfMinusSymbolBasedOnPreviousToken(prev);
    else if (str == "--") return Type::DECREMENT;
    else if (str == "-=") return Type::SUBTRACT_ASSIGN;
    else if (str == "*") return Type::MULTIPLY;
    else if (str == "*=") return Type::MULTIPLY_ASSIGN;
    else if (str == "%") return Type::MODULO;
    else if (str == "%=") return Type::MODULO_ASSIGN;
    else if (str == "/") return Type::DIVIDE;
    else if (str == "/=") return Type::DIVIDE_ASSIGN;
    else if (str == "//") return Type::FLOOR_DIVIDE;
    else if (str == "//=") return Type::FLOOR_DIVIDE_ASSIGN;
    else if (str == "=") return Type::ASSIGN;
    else if (str == "==") return Type::EQUALITY_COMPARISON;
    else if (str == "!=") return Type::INEQUALITY_COMPARISON;
    else if (str == "!") return Type::BOOLEAN_INVERSION;
    else if (str == "<") return Type::LESS_THAN_COMPARISON;
    else if (str == "<=") return Type::LESS_THAN_EQUALS_COMPARISON;
    else if (str == ">") return Type::MORE_THAN_COMPARISON;
    else if (str == ">=") return Type::MORE_THAN_EQUALS_COMPARISON;

    else return Type::INVALID; // str may be a valid token, but it is not a valid symbol
}

// determines what the string str would be typed if it were the value
// for a token; defaults to Type::INVALID if str would not form a valid token
Type getTokenTypeOfCode(const string& str, const Type& prev) {
    if (validStringLiteral(str)) { // string literal
        return Type::STRING_LITERAL;
    } else if (validNumberLiteral(str)) { // number literal
        return Type::NUMBER_LITERAL;
    } else if (Type keywordTT = getTokenTypeOfKeyword(str); keywordTT != Type::INVALID) { // keyword
        return keywordTT;
    } else if (validIdentifier(str)) { // identifier
        return Type::IDENTIFIER;
    } else if (Type symbolTT = getTokenTypeOfSymbol(str, prev); symbolTT != Type::INVALID) { // symbol
        return symbolTT;
    } else {
        return Type::INVALID;
    }
}

// << operator overload for Type enum
ostream& operator<<(ostream& os, const Type& type) {
    string toOstream = "";
    switch (type) {
        case Type::NUMBER_LITERAL: toOstream = "NUMBER_LITERAL"; break;
        case Type::STRING_LITERAL: toOstream = "STRING_LITERAL"; break;
        case Type::IDENTIFIER: toOstream = "IDENTIFIER"; break;
        case Type::OPEN_BRACKET: toOstream = "OPEN_BRACKET"; break;
        case Type::CLOSE_BRACKET: toOstream = "CLOSE_BRACKET"; break;

        case Type::ADD: toOstream = "ADD"; break;
        case Type::ADD_ASSIGN: toOstream = "ADD_ASSIGN"; break;
        case Type::INCREMENT: toOstream = "INCREMENT"; break;

        case Type::SUBTRACT: toOstream = "SUBTRACT"; break;
        case Type::NEGATE: toOstream = "NEGATE"; break;
        case Type::SUBTRACT_ASSIGN: toOstream = "SUBTRACT_ASSIGN"; break;
        case Type::DECREMENT: toOstream = "DECREMENT"; break;

        case Type::MULTIPLY: toOstream = "MULTIPLY"; break;
        case Type::MULTIPLY_ASSIGN: toOstream = "MULTIPLY_ASSIGN"; break;

        case Type::DIVIDE: toOstream = "DIVIDE"; break;
        case Type::DIVIDE_ASSIGN: toOstream = "DIVIDE_ASSIGN"; break;

        case Type::FLOOR_DIVIDE: toOstream = "FLOOR_DIVIDE"; break;
        case Type::FLOOR_DIVIDE_ASSIGN: toOstream = "FLOOR_DIVIDE_ASSIGN"; break;

        case Type::MODULO: toOstream = "MODULO"; break;
        case Type::MODULO_ASSIGN: toOstream = "MODULO_ASSIGN"; break;

        case Type::ASSIGN: toOstream = "ASSIGN"; break;

        case Type::EQUALITY_COMPARISON: toOstream = "EQUALITY_COMPARISON"; break;
        case Type::LESS_THAN_COMPARISON: toOstream = "LESS_THAN_COMPARISON"; break;
        case Type::MORE_THAN_COMPARISON: toOstream = "MORE_THAN_COMPARISON"; break;
        case Type::LESS_THAN_EQUALS_COMPARISON: toOstream = "LESS_THAN_EQUALS_COMPARISON"; break;
        case Type::MORE_THAN_EQUALS_COMPARISON: toOstream = "MORE_THAN_EQUALS_COMPARISON"; break;
        case Type::INEQUALITY_COMPARISON: toOstream = "INEQUALITY_COMPARISON"; break;
        case Type::BOOLEAN_INVERSION: toOstream = "BOOLEAN_INVERSION"; break;

        case Type::VAR_KW: toOstream = "VAR_KW"; break;
        case Type::SHOW_KW: toOstream = "SHOW_KW"; break;
        case Type::IF_KW: toOstream = "IF_KW"; break;
        case Type::ELSE_KW: toOstream = "ELSE_KW"; break;
        case Type::REPEAT_KW: toOstream = "REPEAT_KW"; break;
        case Type::WHILE_KW: toOstream = "WHILE_KW"; break;
        case Type::LOGICAL_AND_KW: toOstream = "LOGICAL_AND_KW"; break;
        case Type::LOGICAL_OR_KW: toOstream = "LOGICAL_OR_KW"; break;

        case Type::INVALID: toOstream = "INVALID"; break;
        case Type::AMBIGUOUS: toOstream = "AMBIGUOUS"; break;
        case Type::END_OF_EXPRESSION: toOstream = "END_OF_EXPRESSION"; break;
        case Type::NONE: toOstream = "NONE"; break;
        default: toOstream = "UNKNOWN_TYPE"; break;
    }

    return (os << toOstream); // return ostream to allow for chaining
}

