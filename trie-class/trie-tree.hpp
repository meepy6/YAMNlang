#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <memory>
#include <string>

namespace trie {
    struct TrieNode {
        std::unordered_map<char, std::unique_ptr<TrieNode>> children;
        bool is_end_of_path = false;
    };

    class Trie {
    public:
        Trie();

        std::unique_ptr<TrieNode> root;
        void insert(const std::string& word);
        bool search(const std::string& word) const;
        bool startsWith(const std::string& prefix) const;
    };
}


#endif
