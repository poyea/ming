//
// ming   C++ containers library
// Copyright (C) 2022-2026 John Law
//
// ming is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ming is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ming.  If not, see <https://www.gnu.org/licenses/>.
//

#ifndef MING_TRIE
#define MING_TRIE

#include <memory>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace ming {

/**
 * @brief A trie data structure for storing and retrieving strings
 *
 * @tparam Key The character type used for storing keys (defaults to char)
 */
template <typename Key = char>
class Trie {
  /**
   * @brief Node class for the trie structure
   */
  class TrieNode {
  public:
    TrieNode() : end_of_word(false){};

    bool end_of_word;
    std::unordered_map<Key, std::unique_ptr<TrieNode>> children;
  };

  /**
   * @brief Helper function to clone a trie structure
   *
   * @param from_node Source node to clone from
   * @param to_node Destination node to clone to
   */
  void clone(std::unique_ptr<TrieNode> const &from_node,
             std::unique_ptr<TrieNode> &to_node) {
    to_node->end_of_word = from_node->end_of_word;
    for (auto &[c, child_node] : from_node->children) {
      auto &dest_child = to_node->children[c];
      if (!dest_child)
        dest_child = std::make_unique<TrieNode>();
      clone(child_node, dest_child);
    }
  }

public:
  /**
   * @brief Construct a new empty Trie
   */
  Trie() noexcept : m_root(std::make_unique<TrieNode>()) {}

  /**
   * @brief Construct a Trie with initial words
   *
   * @param list List of words to insert into the trie
   */
  Trie(std::initializer_list<std::string_view> list)
      : m_root(std::make_unique<TrieNode>()) {
    for (auto const &str : list) {
      insert(str);
    }
  }

  /**
   * @brief Destructor
   */
  ~Trie() noexcept = default;

  /**
   * @brief Copy constructor
   *
   * @param other Trie to copy from
   */
  Trie(Trie const &other) noexcept : Trie() { clone(other.m_root, m_root); }

  /**
   * @brief Move constructor
   *
   * @param other Trie to move from
   */
  Trie(Trie &&other) noexcept : m_root(std::exchange(other.m_root, nullptr)) {}

  /**
   * @brief Copy assignment operator
   *
   * @param other Trie to copy from
   * @return Trie& Reference to this object
   */
  Trie &operator=(Trie const &other) noexcept {
    if (this == &other) {
      return *this;
    }
    return *this = Trie(other);
  }

  /**
   * @brief Move assignment operator
   *
   * @param other Trie to move from
   * @return Trie& Reference to this object
   */
  Trie &operator=(Trie &&other) noexcept {
    std::swap(m_root, other.m_root);
    return *this;
  }

  /**
   * @brief Insert a word into the trie
   *
   * @param word The word to insert
   */
  void insert(std::string_view word) noexcept {
    auto *root = m_root.get();
    for (auto const &c : word) {
      auto it = root->children.find(c);
      if (it == root->children.end()) {
        root->children[c] = std::make_unique<TrieNode>();
      }
      root = root->children[c].get();
    }
    root->end_of_word = true;
  }

  /**
   * @brief Check if a word exists in the trie
   *
   * @param word The word to check for
   * @return true If the word exists in the trie
   * @return false If the word does not exist in the trie
   */
  [[nodiscard]] bool is_word(std::string_view word) const noexcept {
    auto *root = m_root.get();
    for (auto const &c : word) {
      auto it = root->children.find(c);
      if (it == root->children.end()) {
        return false;
      }
      root = it->second.get();
    }
    return root->end_of_word;
  }

  /**
   * @brief Check if any word in the trie starts with the given prefix
   *
   * @param prefix The prefix to check for
   * @return true If the prefix exists in the trie
   * @return false If no word in the trie starts with the prefix
   */
  [[nodiscard]] bool starts_with(std::string_view prefix) const noexcept {
    auto *root = m_root.get();
    for (auto const &c : prefix) {
      auto it = root->children.find(c);
      if (it == root->children.end()) {
        return false;
      }
      root = it->second.get();
    }
    return true;
  }

private:
  /** @brief Root node of the trie */
  std::unique_ptr<TrieNode> m_root;
};

} // namespace ming

#endif // MING_TRIE