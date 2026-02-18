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

#ifndef MING_DISJOINT_SET
#define MING_DISJOINT_SET

#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <utility>

namespace ming {

/**
 * @brief A disjoint-set data structure (also known as union-find)
 *
 * @tparam T The type of elements stored in the set
 */
template <typename T>
class DisjointSet {

  /**
   * @brief Node class for the disjoint set elements
   *
   * @tparam U Type of element stored in the node
   */
  template <typename U>
  class DisjointSetNode {
    friend DisjointSet<T>;

    using node_type = DisjointSetNode<U>;
    using node_ptr = std::shared_ptr<node_type>;
    using weak_node_ptr = std::weak_ptr<node_type>;

    using object_ptr = std::unique_ptr<U>;

  public:
    /**
     * @brief Construct a new DisjointSetNode
     *
     * @param object The object to store
     * @param parent Pointer to parent node (nullptr for root)
     * @param rank The rank of the node (for union by rank)
     */
    DisjointSetNode(object_ptr object, node_ptr parent, int rank)
        : m_object(std::move(object)), m_parent(parent), m_rank(rank) {}

    ~DisjointSetNode() = default;

    DisjointSetNode(DisjointSetNode const &other) noexcept {
      m_object = std::make_unique<U>(*other.m_object);
      if (other.lock_parent() != nullptr) {
        m_parent = other.m_parent;
      } else {
        m_parent = std::weak_ptr<node_type>();
      }
      m_rank = other.m_rank;
    }

    DisjointSetNode(DisjointSetNode &&other) noexcept
        : m_object(std::move(other.m_object)),
          m_parent(std::exchange(other.m_parent, std::weak_ptr<node_type>())),
          m_rank(other.m_rank) {}

    DisjointSetNode &operator=(DisjointSetNode const &other) {
      if (this == &other) {
        return *this;
      }
      return *this = DisjointSetNode(other);
    }

    DisjointSetNode &operator=(DisjointSetNode &&other) noexcept {
      std::swap(m_object, other.m_object);
      std::swap(m_parent, other.m_parent);
      std::swap(m_rank, other.m_rank);
      return *this;
    }

    const T &get_object() const noexcept { return *m_object.get(); }
    T &get_object() noexcept { return *m_object.get(); }

    const weak_node_ptr &get_parent() const { return m_parent; }
    weak_node_ptr &get_parent() { return m_parent; }

    node_ptr lock_parent() const { return m_parent.lock(); }

    std::uint64_t get_rank() const noexcept { return m_rank; }

  private:
    object_ptr m_object;
    weak_node_ptr m_parent;
    std::uint64_t m_rank;
  };

public:
  class Iterator;

  using node_type = typename DisjointSetNode<T>::node_type;
  using node_ptr = typename DisjointSetNode<T>::node_ptr;

  using object_ptr = typename DisjointSetNode<T>::object_ptr;

  /**
   * @brief Compresses the path from node to root for faster future lookups
   *
   * @param node The node to compress path for
   * @param root The root node
   */
  void path_compress(node_ptr &node, node_ptr root) {
    for (auto tmp = node->lock_parent(); tmp && tmp != root;) {
      node->m_parent = root;
      node = tmp;
      tmp = node->lock_parent();
    }
  }

public:
  /**
   * @brief Iterator for DisjointSet elements
   */
  class Iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = node_ptr;
    using pointer = node_ptr *;
    using reference = node_ptr &;

  public:
    Iterator() noexcept : m_pointer(&m_node) {}
    Iterator(pointer ptr) noexcept : m_pointer(ptr) {}
    Iterator(value_type val) noexcept : m_node(std::move(val)), m_pointer(&m_node) {}

    Iterator(Iterator const &other) noexcept
        : m_node(other.m_node), m_pointer(&m_node) {}

    Iterator(Iterator &&other) noexcept
        : m_node(std::move(other.m_node)), m_pointer(&m_node) {}

    pointer operator->() { return m_pointer; }

    reference operator*() const {
      if (!m_pointer) {
        throw std::runtime_error("Dereferencing null DisjointSet Iterator");
      }
      return *m_pointer;
    }

    Iterator &operator++() { throw std::logic_error("Not Implemented"); }

    Iterator operator++(int) {
      Iterator tmp{*this};
      ++(*this);
      return tmp;
    }

    friend bool operator==(Iterator const &a, Iterator const &b) {
      return a.m_pointer == b.m_pointer;
    };

    friend bool operator!=(Iterator const &a, Iterator const &b) {
      return a.m_pointer != b.m_pointer;
    };

  private:
    value_type m_node;
    pointer m_pointer;
  };

public:
  /**
   * @brief Construct a new DisjointSet object
   */
  DisjointSet() = default;

  /**
   * @brief Destructor
   */
  ~DisjointSet() = default;

  /**
   * @brief Insert an element into the disjoint set
   *
   * @tparam Args Constructor argument types
   * @param args Constructor arguments
   * @return Iterator to the newly inserted element
   */
  template <typename... Args>
  [[maybe_unused]] Iterator insert(Args &&...args) {
    auto new_node = std::make_shared<node_type>(
        std::make_unique<T>(std::forward<Args>(args)...), nullptr, 0);
    return Iterator(std::move(new_node));
  }

  /**
   * @brief Find the representative (root) of the set containing the given element
   *
   * @param object Iterator to the element
   * @return Iterator to the root element
   */
  Iterator find(Iterator object) {
    auto object_ptr = *object;
    while (auto parent = object_ptr->lock_parent()) {
      object_ptr = parent;
    }
    return Iterator(object_ptr);
  }

  [[nodiscard]] bool are_same_set(Iterator node_a, Iterator node_b) {
    Iterator parent_a = find(node_a), parent_b = find(node_b);
    Iterator *const root_a = &parent_a, *const root_b = &parent_b;
    path_compress(*node_a, **root_a);
    path_compress(*node_b, **root_b);
    return **root_a == **root_b;
  }

  /**
   * @brief Merge two sets together
   *
   * @param node_a_iter Iterator to element in first set
   * @param node_b_iter Iterator to element in second set
   */
  void merge(Iterator const node_a_iter, Iterator const node_b_iter) {
    node_ptr node_a = *node_a_iter;
    node_ptr node_b = *node_b_iter;

    Iterator parent_a_iter = find(node_a_iter);
    Iterator parent_b_iter = find(node_b_iter);
    node_ptr root_a = *parent_a_iter;
    node_ptr root_b = *parent_b_iter;

    if (root_a == root_b) {
      return;
    }

    node_ptr new_root;
    if (root_a->m_rank >= root_b->m_rank) {
      new_root = root_a;
      root_b->m_parent = new_root;
      new_root->m_rank += (root_a->m_rank == root_b->m_rank);
    } else {
      new_root = root_b;
      root_a->m_parent = new_root;
    }

    path_compress(node_a, new_root);
    path_compress(node_b, new_root);
  }
};

} // namespace ming

#endif // MING_DISJOINT_SET