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

#ifndef MING_FENWICK_TREE
#define MING_FENWICK_TREE

#include <atomic>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <new>
#include <string_view>
#include <vector>

namespace ming {

template <class T>
concept FenwickValue = std::integral<T> && (!std::same_as<T, bool>);

struct FenwickError {
  std::string_view message;
};

template <FenwickValue T = std::int64_t>
class FenwickTree {
  static constexpr std::size_t cache_line = std::hardware_destructive_interference_size;

  struct alignas(cache_line) Cell {
    std::atomic<T> value{0};
  };

public:
  explicit FenwickTree(std::size_t n) : m_size(n), m_binary_indexed_tree(n + 1) {}

  explicit FenwickTree(std::vector<T> const &init) : FenwickTree(init.size()) {
    for (std::size_t i = 0; i < init.size(); ++i) {
      add_unchecked(i, init[i]);
    }
  }

  [[nodiscard]] std::size_t size() const noexcept { return m_size; }

  [[nodiscard]] std::expected<void, FenwickError> add(std::size_t index0,
                                                      T delta) noexcept {
    if (index0 >= m_size) {
      return std::unexpected(FenwickError{"index out of range"});
    }
    add_unchecked(index0, delta);
    return {};
  }

  [[nodiscard]] std::expected<T, FenwickError>
  prefix_sum(std::size_t r0) const noexcept {
    if (r0 >= m_size) {
      return std::unexpected(FenwickError{"index out of range"});
    }
    return prefix_sum_unchecked(r0);
  }

  [[nodiscard]] std::expected<T, FenwickError> sum(std::size_t l0,
                                                   std::size_t r0) const noexcept {
    if (l0 > r0 || r0 >= m_size) {
      return std::unexpected(FenwickError{"invalid range"});
    }
    T right = prefix_sum_unchecked(r0);
    T left = (l0 == 0) ? T{0} : prefix_sum_unchecked(l0 - 1);
    return right - left;
  }

  void add_unchecked(std::size_t index0, T delta) noexcept {
    std::size_t i = index0 + 1;
    while (i <= m_size) {
      m_binary_indexed_tree[i].value.fetch_add(delta, std::memory_order_relaxed);
      i += lsb(i);
    }
  }

  [[nodiscard]] T prefix_sum_unchecked(std::size_t r0) const noexcept {
    T acc = 0;
    std::size_t i = r0 + 1;
    while (i != 0) {
      acc += m_binary_indexed_tree[i].value.load(std::memory_order_relaxed);
      i -= lsb(i);
    }
    return acc;
  }

private:
  [[nodiscard]] static constexpr std::size_t lsb(std::size_t x) noexcept {
    return x & (~x + 1);
  }

private:
  std::size_t m_size;
  std::vector<Cell> m_binary_indexed_tree;
};

} // namespace ming

#endif // MING_FENWICK_TREE