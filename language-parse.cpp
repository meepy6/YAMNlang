#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>
#include "type-enum.hpp"
#include "trie-class/trie-tree.hpp"
using namespace std;
using namespace trie;

#define DEBUG_MODE (uncomment for debugging logs in parse function

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

Trie generateTrieFromStrings(vector<string> strings) {
    Trie newTrie = Trie();

    for (const auto& str : strings) {
        newTrie.insert(str);
    }

    return newTrie;
}

vector<Token> parse(string code) {
    // initialise symbol trie
    static const Trie symbolTrie = generateTrieFromStrings(valid_symbols);

    // initialise tokens vector
    vector<Token> tokens = { };

    // parsing loop (uses while and manual increment to facilitate skipping ahead when we come across a literal or identifier)
    size_t i = 0;
    while (i < code.length()) {
        // the current character (unsigned so i can avoid writing static_cast<unsigned char>(char) multiple times (some <cctype> functions require the conversion)
        unsigned char curr = code[i];

        // identifier/keyword
        if (isalpha(curr) || curr == '_') {
            string word = string(1, curr);

            ++i;
            // consume characters until we hit a non-alphanumeric character that is not '_' ([a-z][A-Z][0-9]_)
            while ((isalnum(static_cast<unsigned char>(code[i])) || code[i] == '_') && i < code.length()) {
                word.push_back(code[i]);
                ++i;
            }

            // check if keyword_strings_to_types contain word
            auto searchForWordInKeywords = keyword_strings_to_types.find(word);

            // if so, push that keyword
            if (searchForWordInKeywords != keyword_strings_to_types.end()) {
                tokens.push_back(Token{keyword_strings_to_types[word], word});
            } else {
                // otherwise, word is Type::IDENTIFIER
                tokens.push_back(Token{Type::IDENTIFIER, word});
            }

            continue;
        }

        // number literal
        if (isdigit(curr)) {
            string word = string(1, curr);

            ++i;
            // consume characters until we hit a non-digit character
            while (isdigit(static_cast<unsigned char>(code[i]))) {
                word.push_back(code[i]);
                ++i;
            }

            // push numeric literal
            tokens.push_back(Token{Type::NUMBER_LITERAL, word});
            continue;
        }

        // string literal
        if (curr == '"') {
            string word = string(1, curr);

            ++i;
            // consume characters until closing quote (")
            while (code[i] != '"') {
                word.push_back(code[i]);
                ++i;
            }

            // push closing quote!!!
            word.push_back(code[i]);

            // push string literal
            tokens.push_back(Token{Type::STRING_LITERAL, word});
        }

        // symbols
        if (symbolTrie.startsWith(string(1, curr))) {
            string word = "";
            TrieNode* current = symbolTrie.root.get();

            while (true) {
                char nextCh = code[i];

                if (current->children.find(nextCh) != current->children.end()) {
                    current = &(current->children[nextCh]); // advance down the tree
                    word.push_back(nextCh);
                    ++i;
                } else {
                    Type type = current->is_end_of_path ? getTokenTypeOfString(word, tokens.back().type) : Type::INVALID;
                    tokens.push_back(Token{type, word});
                    break;
                }
            }
        }

        ++i;
    }

    return tokens;
}

int main() {
    string code = "var coolString = \"Hello, world!\"\nif (coolString != \"Good morning\")";
    // parser function
    vector<Token> parsedTokens = parse(code);

    for (const auto& token : parsedTokens) {
        cout << token << endl;
    }
}
