#pragma once

#include <memory>
#include <string_view>
#include <unordered_map>

namespace ming {

class Trie {
  class TrieNode {
  public:
    TrieNode() : end_of_word(false){};

    bool end_of_word;
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
  };

  void clone(std::unique_ptr<TrieNode> &from_node, std::unique_ptr<TrieNode> &to_node) {
    for (auto &[c, child_node] : from_node->children) {
      auto it = to_node->children.find(c);
      if (it == to_node->children.end()) {
        to_node->children[c] = std::make_unique<TrieNode>();
        if (child_node->end_of_word) {
          to_node->end_of_word = true;
        }
        clone(child_node, to_node->children[c]);
      } else {
        if (child_node->end_of_word) {
          it->second->end_of_word = true;
        }
        clone(child_node, it->second);
      }
    }
  }

public:
  Trie() : m_root(std::make_unique<TrieNode>()) {}

  ~Trie() {}

  Trie(const Trie &other) : Trie() {
    for (auto &[c, child_node] : other.m_root->children) {
      auto it = m_root->children.find(c);
      if (it == m_root->children.end()) {
        m_root->children[c] = std::make_unique<TrieNode>();
        clone(child_node, m_root->children[c]);
      } else {
        clone(child_node, it->second);
      }
    }
  }

  Trie(Trie &&other) noexcept : m_root(std::exchange(other.m_root, nullptr)) {}

  Trie &operator=(const Trie &other) { return *this = Trie(other); }

  Trie &operator=(Trie &&other) noexcept {
    std::swap(m_root, other.m_root);
    return *this;
  }

  void insert(std::string_view word) {
    auto *root = m_root.get();
    for (auto &c : word) {
      auto it = root->children.find(c);
      if (it == root->children.end()) {
        root->children[c] = std::make_unique<TrieNode>();
      }
      root = root->children[c].get();
    }
    root->end_of_word = true;
  }

  bool search(std::string_view word) {
    auto *root = m_root.get();
    for (auto &c : word) {
      auto it = root->children.find(c);
      if (it == root->children.end()) {
        return false;
      }
      root = it->second.get();
    }
    return root->end_of_word;
  }

  bool starts_with(std::string_view prefix) {
    auto *root = m_root.get();
    for (auto &c : prefix) {
      auto it = root->children.find(c);
      if (it == root->children.end()) {
        return false;
      }
      root = it->second.get();
    }
    return true;
  }

private:
  std::unique_ptr<TrieNode> m_root;
};

} // namespace ming
