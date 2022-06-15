#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace ming {

class Trie {
  class TrieNode {
  public:
    TrieNode() : is_end(false){};

    bool is_end;
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
  };

  void clone(std::unique_ptr<TrieNode> &from_node, std::unique_ptr<TrieNode> &to_node) {
    for (auto &[c, child_node] : from_node->children) {
      auto it = to_node->children.find(c);
      if (it == to_node->children.end()) {
        to_node->children[c] = std::make_unique<TrieNode>();
        if (child_node->is_end) {
          to_node->is_end = true;
        }
        clone(child_node, to_node->children[c]);
      } else {
        if (child_node->is_end) {
          it->second->is_end = true;
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

  void insert(std::string word) {
    auto *root = m_root.get();
    for (auto &c : word) {
      auto it = root->children.find(c);
      if (it == root->children.end()) {
        root->children[c] = std::make_unique<TrieNode>();
      }
      root = root->children[c].get();
    }
    root->is_end = true;
  }

  bool search(std::string word) {
    auto *root = m_root.get();
    for (auto &c : word) {
      auto it = root->children.find(c);
      if (it == root->children.end()) {
        return false;
      }
      root = it->second.get();
    }
    return root->is_end;
  }

  bool starts_with(std::string prefix) {
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
