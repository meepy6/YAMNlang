#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>
#include "type-enum.hpp"
using namespace std;

// #define DEBUG_MODE (uncomment for debugging logs in parse function

#ifdef DEBUG_MODE
    #define LOG(x) cout << x
#else
    #define LOG(x)
#endif // DEBUG_MODE

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
    size_t i = 0;
    string current = "";
    while (i < code.length()) {
        // the current character
        char curr = code[i];
        LOG("curr (a.k.a. code[i]) = " << curr << endl);

        // updCurrent = current with curr appended
        string updCurrent = current;
        updCurrent.push_back(curr);

        LOG("current = " << current << ", updCurrent = " << updCurrent << endl);

        // previous token type; if tokens is empty, defaults to Type::NONE
        const Type prevToken = tokens.empty() ? Type::NONE : tokens[tokens.size() - 1].type;

        // updCurrent would have Type::INVALID if it were pushed to tokens
        if (getTokenTypeOfString(updCurrent, prevToken) == Type::INVALID) {
            // get type of current
            Type currType = getTokenTypeOfString(current, prevToken);
            if (currType != Type::INVALID) {
                // current would have a Type != Type::INVALID;
                // push to tokens as current represents the longest
                // valid string from the after the last token
                tokens.push_back(Token{currType, current});
            }

            // reset value of current to be curr
            // (but convert to std::string so we can add to it later)
            current = string(1, curr);

            LOG("current reset to = " << current << endl);
        } else {
            // updCurrent would have a Type != Type::INVALID
            // update current to include the newest character (curr)
            // and continue
            current = updCurrent;
        }

        ++i;
        LOG(endl);
    }

    // if current would have a Type != Type::INVALID
    // after the loop ends, push it as a token
    if (!current.empty()) {
        // previous token type; if tokens is empty, defaults to Type::NONE
        const Type prevToken = tokens.empty() ? Type::NONE : tokens[tokens.size() - 1].type;

        // current token type
        Type currType = getTokenTypeOfString(current, prevToken);

        // if currType != Type::INVALID, push a new token
        if (currType != Type::INVALID) {
            tokens.push_back(Token{currType, current});
        }
    }

    return tokens;
}

int main() {
    string code = "var coolNumber = 4\nshow coolNumber\nif (coolNumber != 4 or coolNumber < -1)";
    // parser function
    vector<Token> parsedTokens = parse(code);

    for (const auto& token : parsedTokens) {
        cout << token << endl;
    }
}
