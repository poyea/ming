//
// ming/disjoint_set_union.hpp   Slightly advanced containers for C++
// Copyright (C) 2022  John Law
//
// ming/disjoint_set_union.hpp is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ming/disjoint_set_union.hpp is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ming/disjoint_set_union.hpp.  If not, see <https://www.gnu.org/licenses/>.
//

#pragma once

#include <memory>

namespace ming {

template <typename T>
class DisjointSetUnion;
template <typename T>
class DisjointSetUnionNode;

template <typename T>
class DisjointSetUnionNode {
  friend DisjointSetUnion<T>;

  using node_type = DisjointSetUnionNode<T>;
  using node_ptr = std::shared_ptr<node_type>;

  using object_ptr = std::unique_ptr<T>;

public:
  DisjointSetUnionNode(object_ptr object, node_ptr parent, int rank)
      : m_object(std::move(object)), m_parent(std::move(parent)), m_rank(rank) {}

  ~DisjointSetUnionNode() = default;

  DisjointSetUnionNode(const DisjointSetUnionNode &other) {
    m_object = std::make_unique<T>(*other.m_object);
    if (other.m_parent != nullptr) {
      m_parent = std::make_shared<node_type>(*other.m_parent);
    } else {
      m_parent = nullptr;
    }
    m_rank = other.m_rank;
  }

  DisjointSetUnionNode(DisjointSetUnionNode &&other) noexcept
      : m_object(std::move(other.m_object)),
        m_parent(std::exchange(other.m_parent, nullptr)), m_rank(other.m_rank) {}

  DisjointSetUnionNode &operator=(const DisjointSetUnionNode &other) {
    if (this == &other) {
      return *this;
    }
    return *this = DisjointSetUnionNode(other);
  }

  DisjointSetUnionNode &operator=(DisjointSetUnionNode &&other) noexcept {
    std::swap(m_object, other.m_object);
    std::swap(m_parent, other.m_parent);
    std::swap(m_rank, other.m_rank);
    return *this;
  }

  const T &get_object() const noexcept { return *m_object.get(); }
  T &get_object() noexcept { return *m_object.get(); }

  const node_ptr &get_parent() const { return m_parent; }
  node_ptr &get_parent() { return m_parent; }

  int get_rank() const noexcept { return m_rank; }

private:
  object_ptr m_object;
  node_ptr m_parent;
  int m_rank;
};

template <typename T>
class DisjointSetUnion {
public:
  using node_type = DisjointSetUnionNode<T>::node_type;
  using node_ptr = DisjointSetUnionNode<T>::node_ptr;

  using object_ptr = DisjointSetUnionNode<T>::object_ptr;

  void path_compress(node_ptr &node, node_ptr root) {
    for (node_ptr tmp = node->m_parent; tmp && tmp != root;) {
      node->m_parent = root;
      node = tmp;
      tmp = node->m_parent;
    }
  }

public:
  DisjointSetUnion() = default;

  ~DisjointSetUnion() = default;

  template <typename... Args>
  [[maybe_unused]] node_ptr insert(Args &&... args) {
    auto new_node = std::make_shared<node_type>(
        std::make_unique<T>(std::forward<Args>(args)...), nullptr, 0);
    return new_node;
  }

  node_ptr find(node_ptr object) {
    node_ptr *root = &object;
    while ((*root)->m_parent != nullptr) {
      root = &((*root)->m_parent);
    }
    return *root;
  }

  [[nodiscard]] bool are_same_set(node_ptr node_a, node_ptr node_b) {
    node_ptr parent_a = find(node_a), parent_b = find(node_b);
    node_ptr *const root_a = &parent_a, *const root_b = &parent_b;
    path_compress(node_a, *root_a);
    path_compress(node_b, *root_b);
    return *root_a == *root_b;
  }

  void merge(node_ptr node_a, node_ptr node_b) {
    node_ptr parent_a = find(node_a), parent_b = find(node_b);
    node_ptr *const root_a = &parent_a, *const root_b = &parent_b;
    node_ptr *new_root;
    if ((*root_a)->m_rank >= (*root_b)->m_rank) {
      new_root = root_a;
      (*root_b)->m_parent = *new_root;
      (*new_root)->m_rank += ((*root_a)->m_rank == (*root_b)->m_rank);
    } else {
      new_root = root_b;
      (*root_a)->m_parent = *new_root;
    }
    path_compress(node_a, *new_root);
    path_compress(node_b, *new_root);
  }
};

} // namespace ming
