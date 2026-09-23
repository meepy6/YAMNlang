#include <unordered_map>
#include <memory>
#include <string>
#include "trie-tree.hpp"

namespace trie {
    Trie::Trie() : root(std::make_unique<TrieNode>()) {}

    void Trie::insert(const std::string& word) {
        TrieNode* current = Trie::root.get();

        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = std::make_unique<TrieNode>();
            }

            current = current->children[ch].get();
        }

        current->is_end_of_path = true;
    }

    bool Trie::search(const std::string& word) const {
        TrieNode* current = Trie::root.get();

        for (char ch : word) {
            auto it = current->children.find(ch);
            if (it == current->children.end()) return false;
            current = it->second.get();
        }

        return current->is_end_of_path;
    }

    bool Trie::startsWith(const std::string& prefix) const {
        TrieNode* current = Trie::root.get();

        for (char ch : prefix) {
            auto it = current->children.find(ch);
            if (it == current->children.end()) return false;
            current = it->second.get();
        }

        return true;
    }
}
