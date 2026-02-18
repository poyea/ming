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

#ifndef MING_SKIPLIST_HPP
#define MING_SKIPLIST_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <random>
#include <vector>

namespace ming {

/**
 * @brief A probabilistic data structure that allows O(log n) search complexity
 * and O(log n) insertion complexity within an ordered sequence
 *
 * @see https://dl.acm.org/doi/10.1145/78973.78977
 *
 * @tparam Key The key type used for comparison and ordering
 * @tparam T The value type stored in the skip list
 * @tparam Compare The comparison function type
 */
template <typename Key, typename T, typename Compare = std::less<Key>>
class SkipList {
private:
  static constexpr int MAX_LEVEL = 32; // for now...

  // Probability factor for level generation (kinda textbook definition)
  static constexpr float P = 0.5f;

  struct Node {
    Key key;
    T value;
    // Each node owns itself through shared_ptr references from predecessors at each
    // level. Forward pointers are shared_ptr so multiple levels can reference the same
    // node safely.
    std::vector<std::shared_ptr<Node>> forward;

    Node(Key const &k, T const &v, int level)
        : key(k), value(v), forward(static_cast<size_t>(level)) {}

    Node(Key &&k, T &&v, int level)
        : key(std::move(k)), value(std::move(v)), forward(static_cast<size_t>(level)) {}
  };

  using node_ptr = std::shared_ptr<Node>;
  node_ptr m_head;

  int m_level;

  size_t m_size;

  Compare m_compare;

  std::mt19937 m_gen;
  std::uniform_real_distribution<float> m_dis;

  int get_random_level() noexcept {
    int generated_level = 1;
    while (m_dis(m_gen) < P && generated_level < MAX_LEVEL)
      ++generated_level;
    return generated_level;
  }

  /**
   * @brief Descend along a given level with bounds + key comparison checks. Walk
   * forward while next node exists and key(next) < target key.
   *
   * @param node The current node to descend from
   * @param level The level to descend at
   * @param key The target key to compare against
   * @return The last node encountered before the target key
   */
  Node *m_descend(Node *node, int level, Key const &key) const noexcept {
    while (level < static_cast<int>(node->forward.size()) && node->forward[level] &&
           m_compare(node->forward[level]->key, key)) {
      node = node->forward[level].get();
    }
    return node;
  }

public:
  /**
   * @brief Construct a new SkipList object
   */
  SkipList()
      : m_head(std::make_shared<Node>(Key{}, T{}, MAX_LEVEL)), m_level(1), m_size(0),
        m_compare(Compare{}), m_gen(std::random_device{}()), m_dis(0.0, 1.0) {}

  /**
   * @brief Construct a new SkipList with custom comparator
   *
   * @param comp The comparison function object
   */
  explicit SkipList(Compare comp)
      : m_head(std::make_shared<Node>(Key{}, T{}, MAX_LEVEL)), m_level(1), m_size(0),
        m_compare(std::move(comp)), m_gen(std::random_device{}()), m_dis(0.0, 1.0) {}

  /**
   * @brief Insert a key-value pair into the skiplist
   *
   * @param key The key
   * @param value The value
   * @return true If inserted successfully
   * @return false If not inserted (e.g., duplicate key)
   */
  bool insert(Key const &key, T const &value) {
    std::vector<Node *> update(MAX_LEVEL);
    Node *current = m_head.get();

    // Find the position to insert at each level (top-down)
    for (int i = m_level - 1; i >= 0; --i) {
      current = m_descend(current, i, key);
      update[i] = current;
    }

    current = (!current->forward.empty()) ? current->forward[0].get() : nullptr;

    // Reject duplicate key (no insertion performed)
    if (current && !m_compare(current->key, key) && !m_compare(key, current->key)) {
      return false;
    }

    int new_level = get_random_level();

    if (new_level > m_level) {
      for (int i = m_level; i < new_level; i++) {
        update[i] = m_head.get();
      }
      m_level = new_level;
    }

    auto new_node = std::make_shared<Node>(key, value, new_level);

    // Insert node at the appropriate position at every level
    for (int i = 0; i < new_level; i++) {
      new_node->forward[i] = update[i]->forward[i];
      update[i]->forward[i] = new_node; // share ownership
    }

    m_size++;
    return true;
  }

  /**
   * @brief Search for a key in the skiplist
   *
   * @param key The key to search for
   * @param value Reference to store the found value
   * @return true If key was found
   * @return false If key was not found
   */
  [[nodiscard]] bool search(Key const &key, T &value) const {
    Node *current = m_head.get();

    // Search from the highest level downwards
    for (int i = m_level - 1; i >= 0; --i) {
      current = m_descend(current, i, key);
    }

    current = (!current->forward.empty()) ? current->forward[0].get() : nullptr;

    // Check if we found the key
    if (current && !m_compare(current->key, key) && !m_compare(key, current->key)) {
      value = current->value;
      return true;
    }

    return false;
  }

  /**
   * @brief Check if a key exists in the skiplist
   *
   * @param key The key to check for
   * @return true If key exists
   * @return false If key does not exist
   */
  [[nodiscard]] bool contains(Key const &key) const {
    T dummy;
    return search(key, dummy);
  }

  /**
   * @brief Remove a key from the skiplist
   *
   * @param key The key to remove
   * @return true If key was found and removed
   * @return false If key was not found
   */
  bool erase(Key const &key) {
    std::vector<Node *> update(MAX_LEVEL);
    Node *current = m_head.get();

    // Find the node to remove at each level
    for (int i = m_level - 1; i >= 0; --i) {
      current = m_descend(current, i, key);
      update[i] = current;
    }

    current = (!current->forward.empty()) ? current->forward[0].get() : nullptr;

    // If key was found, remove it
    if (current && !m_compare(current->key, key) && !m_compare(key, current->key)) {
      // Update pointers at each level
      for (int i = 0; i < m_level; i++) {
        if (update[i]->forward[i].get() != current) {
          break;
        }
        update[i]->forward[i] = current->forward[i];
      }

      // Update the level if needed
      while (m_level > 1 && !m_head->forward[m_level - 1]) {
        m_level--;
      }

      m_size--;
      return true;
    }

    return false;
  }

  /**
   * @brief Get the size of the skiplist
   *
   * @return size_t Number of elements in the skiplist
   */
  [[nodiscard]] size_t size() const noexcept { return m_size; }

  /**
   * @brief Check if the skiplist is empty
   *
   * @return true If the skiplist has no elements
   * @return false If the skiplist has at least one element
   */
  [[nodiscard]] bool empty() const noexcept { return m_size == 0; }

  /**
   * @brief Iterator for traversing the skiplist
   */
  class iterator {
  private:
    Node *current;

  public:
    using value_type = std::pair<const Key &, T &>;
    using reference = value_type;
    using pointer = void;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;
    iterator(Node *node) noexcept : current(node) {}

    iterator &operator++() noexcept {
      if (current) {
        current = current->forward[0].get();
      }
      return *this;
    }

    iterator operator++(int) noexcept {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(iterator const &other) const noexcept {
      return current == other.current;
    }

    bool operator!=(iterator const &other) const noexcept { return !(*this == other); }

    value_type operator*() const noexcept { return {current->key, current->value}; }
  };

  /**
   * @brief Get an iterator to the first element
   *
   * @return iterator
   */
  iterator begin() noexcept { return iterator(m_head->forward[0].get()); }

  /**
   * @brief Get an iterator to the end
   *
   * @return iterator
   */
  iterator end() noexcept { return iterator(nullptr); }

  /**
   * @brief Find a key in the skiplist
   *
   * @param key The key to find
   * @return iterator Iterator pointing to the found key or end() if not found
   */
  [[nodiscard]] iterator find(Key const &key) {
    Node *current = m_head.get();

    for (int i = m_level - 1; i >= 0; --i) {
      current = m_descend(current, i, key);
    }

    current = (!current->forward.empty()) ? current->forward[0].get() : nullptr;

    if (current && !m_compare(current->key, key) && !m_compare(key, current->key)) {
      return iterator(current);
    }

    return end();
  }
};

} // namespace ming

#endif // MING_SKIPLIST_HPP
