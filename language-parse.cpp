#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>
#include "type-enum.hpp"
using namespace std;


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
        char curr = code[i];
        string updCurrent = current;
        updCurrent.push_back(curr);

        const Type prevToken = tokens.empty() ? Type::NONE : tokens[tokens.size() - 1].type;

        // current + curr is invalid
        if (getTokenTypeOfString(updCurrent, prevToken) == Type::INVALID) {
            // type of current
            Type currType = getTokenTypeOfString(current, prevToken);
            if (currType != Type::INVALID) {
                // current is valid; push to tokens
                tokens.push_back(Token{currType, current});
            }

            // reset value of current
            current = string(1, curr);
        } else {
            current = updCurrent;
        }

        ++i;
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
