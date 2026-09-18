/*
TODO:

 - prefix tree for operators
 - update << operator override for Type enum
 - update findContextOfMinusSymbolBasedOnPreviousToken to include cases for new tokens
*/

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

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
    SUBTRACT_ASSIGN, // new
    DECREMENT, // new

    MULTIPLY,
    MULTIPLY_ASSIGN, // new

    DIVIDE,
    DIVIDE_ASSIGN, // new

    FLOOR_DIVIDE, // new
    FLOOR_DIVIDE_ASSIGN // new

    MODULO,
    MODULO_ASSIGN, // new

    NEGATE,

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
    LOGICAL_OR_KW // new

    // invalid
    INVALID,

    // ambiguous
    AMBIGUOUS,

    // end of expression
    END_OF_EXPRESSION
};

Type findContextOfMinusSymbolBasedOnPreviousToken(Type prev) {
    switch (prev) {
        // subtract
        case Type::CLOSE_BRACKET:
        case Type::IDENTIFIER:
        case Type::NUMBER_LITERAL: return Type::SUBTRACT;

        // negate
        case Type::SHOW_KW:
        case Type::VAR_KW:

        // shouldn't ever exist anyway, but I don't like compiler warnings LOL
        case Type::NONE:

        case Type::ADD:
        case Type::SUBTRACT:
        case Type::MULTIPLY:
        case Type::DIVIDE:
        case Type::MODULO:
        case Type::ASSIGN:
        case Type::NEGATE:

        case Type::OPEN_BRACKET:
        case Type::END_OF_EXPRESSION: return Type::NEGATE;

        // ambiguous (cannot be determined)
        case Type::INVALID:
        case Type::AMBIGUOUS: return Type::AMBIGUOUS; break;
    }
}

// << operator overload for Type enum
ostream& operator<<(ostream& os, const Type& type) {
    string toOstream = "";
    switch (type) {
        case Type::NUMBER_LITERAL: toOstream = "NUMBER_LITERAL"; break;
        case Type::IDENTIFIER: toOstream = "IDENTIFIER"; break;
        case Type::OPEN_BRACKET: toOstream = "OPEN_BRACKET"; break;
        case Type::CLOSE_BRACKET: toOstream = "CLOSE_BRACKET"; break;
        case Type::ADD: toOstream = "ADD"; break;
        case Type::SUBTRACT: toOstream = "SUBTRACT"; break;
        case Type::MULTIPLY: toOstream = "MULTIPLY"; break;
        case Type::DIVIDE: toOstream = "DIVIDE"; break;
        case Type::MODULO: toOstream = "MODULO"; break;
        case Type::NEGATE: toOstream = "NEGATE"; break;
        case Type::ASSIGN: toOstream = "ASSIGN"; break;
        case Type::VAR_KW: toOstream = "VAR_KW"; break;
        case Type::SHOW_KW: toOstream = "SHOW_KW"; break;
        case Type::INVALID: toOstream = "INVALID"; break;
        case Type::END_OF_EXPRESSION: toOstream = "END_OF_EXPRESSION"; break;
        case Type::NONE: toOstream = "NONE"; break;
        default: toOstream = "UNKNOWN_TYPE"; break;
    }

    return (os << toOstream); // return ostream to allow for chaining
}

struct Token {
    Type type;
    string value;
};

// << operator overload for Token struct
ostream& operator<<(ostream& os, const Token& token) {
    os << "Token(" << token.type << ", \"" << token.value << "\")";
    return os;
}

vector<Token> parse(string code) {
    // initialise tokens vector
    vector<Token> tokens = { };

    // parsing loop (uses while and manual increment to facilitate skipping ahead when we come across a literal or identifier)
    int i = 0;
    while (i < code.length()) {
        char curr = code[i];

        // newlines act as EOL/End Of Expression indicators, so detect them here
        if (curr == '\n' || curr == '\r') {
            // only push an END_OF_EXPRESSION token if the last token isn't already
            // an END_OF_EXPRESSION
            if (!tokens.empty() && tokens.back().type != Type::END_OF_EXPRESSION) {
                tokens.push_back(Token{Type::END_OF_EXPRESSION, "\\n"});
            }

            ++i;
            continue;
        }

        // skip whitespace
        if (isspace(static_cast<unsigned char>(curr))) {
            ++i;
            continue;
        }

        // identifiers and keywords
        if (isalpha(static_cast<unsigned char>(curr)) || curr == '_') {
            string acc = "";

            while (i < code.length() && (isalnum(static_cast<unsigned char>(code[i])) || code[i] == '_')) {
                acc.push_back(code[i]);
                ++i;
            }

            // check if acc is a keyword, otherwise acc is an identifier
            Type toPush = Type::INVALID;
            if (acc == "var") toPush = Type::VAR_KW;
            else if (acc == "show") toPush = Type::SHOW_KW;
            else toPush = Type::IDENTIFIER;

            tokens.push_back(Token {toPush, acc});
            continue;
        }

        // numeric literals
        if (isdigit(static_cast<unsigned char>(curr))) {
            string acc = "";

            while (i < code.length() && isdigit(static_cast<unsigned char>(code[i]))) {
                acc.push_back(code[i]);
                ++i;
            }

            // push numeric literal and continue, since i has already been incremented
            tokens.push_back(Token {Type::NUMBER_LITERAL, acc});
            continue;
        }

        // single width symbols
        Type toPush = Type::NONE;
        switch (curr) {
            // addition
            case '+': toPush = Type::ADD; break;
            // multiplication
            case '*': toPush = Type::MULTIPLY; break;
            // division
            case '/': toPush = Type::DIVIDE; break;
            // opening bracket
            case '(': toPush = Type::OPEN_BRACKET; break;
            // closing bracket
            case ')': toPush = Type::CLOSE_BRACKET; break;
            // modulus
            case '%': toPush = Type::MODULO; break;
            // assignment operator
            case '=': toPush = Type::ASSIGN; break;
        }

        // subtract/negate (needs custom logic to disambiguate)
        if (curr == '-') {
            // tokens is empty: type must be NEGATE
            if (tokens.empty()) {
                toPush = Type::NEGATE;
            } else { // tokens is not empty
                const Type lastType = tokens.back().type;

                toPush = findContextOfMinusSymbolBasedOnPreviousToken(lastType);
            }
        }

        // if toPush still equals none, curr char must be invalid
        if (toPush == Type::NONE) {
            toPush = Type::INVALID;
        }

        tokens.push_back(Token{toPush, string(1, curr)});

        ++i;
    }

    return tokens;
}

int main() {
    string code = "-5 * --6 / 3 + (5 - 2) * 4 -- 2";
    // parser function
    vector<Token> parsedTokens = parse(code);

    for (const auto& token : parsedTokens) {
        cout << token << endl;
    }
}
