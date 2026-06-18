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

#ifndef MING_RING_BUFFER
#define MING_RING_BUFFER

#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

namespace ming {

template <typename T>
class RingBuffer {
  std::vector<T> m_buffer;
  std::size_t m_head{0};
  std::size_t m_tail{0};
  std::size_t m_capacity{0};

public:
  explicit RingBuffer(std::size_t capacity = 0)
      : m_buffer(capacity), m_capacity(capacity) {
    if (capacity == 0) {
      throw std::runtime_error("RingBuffer capacity is zero!");
    }
  }

  ~RingBuffer() = default;

  RingBuffer(RingBuffer const &other) = default;
  RingBuffer(RingBuffer &&other) noexcept = default;

  RingBuffer &operator=(RingBuffer const &other) = default;
  RingBuffer &operator=(RingBuffer &&other) noexcept = default;

  void push(T const &item) noexcept(std::is_nothrow_copy_assignable_v<T>) {
    if (is_full()) {
      m_tail = (m_tail + 1) % m_capacity;
    }

    m_buffer[m_head] = item;
    m_head = (m_head + 1) % m_capacity;
  }

  T pop() {
    if (is_empty()) {
      throw std::runtime_error("RingBuffer is empty!");
    }

    T item = m_buffer[m_tail];
    m_tail = (m_tail + 1) % m_capacity;
    return item;
  }

  void clear() noexcept {
    m_head = 0;
    m_tail = 0;
  }

  bool is_empty() const noexcept { return m_head == m_tail; }

  bool is_full() const noexcept { return (m_head + 1) % m_capacity == m_tail; }

  std::size_t size() const noexcept {
    if (is_full())
      return m_capacity;
    if (m_head >= m_tail) {
      return m_head - m_tail;
    } else {
      return m_capacity - (m_tail - m_head);
    }
  }

  std::size_t capacity() const noexcept { return m_capacity; }

  void resize(std::size_t new_capacity) {
    if (new_capacity == 0) {
      throw std::runtime_error("RingBuffer capacity is zero!");
    }

    std::vector<T> new_buffer(new_capacity);
    std::size_t new_size = std::min(size(), new_capacity);

    for (std::size_t i = 0; i < new_size; ++i) {
      new_buffer[i] = m_buffer[(m_tail + i) % m_capacity];
    }

    m_buffer = std::move(new_buffer);
    m_capacity = new_capacity;
    m_head = new_size;
    m_tail = 0;
  }
};

} // namespace ming

#endif // MING_RING_BUFFER