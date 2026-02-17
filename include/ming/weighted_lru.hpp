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

#ifndef MING_WLRU
#define MING_WLRU

#include <expected>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace ming {

template <typename K, typename V>
class WeightedLRU {
  struct LRUItem {
    K key;
    V value;
    int size;
  };

  int m_capacity, m_current_cap;
  std::list<LRUItem> m_store;
  std::unordered_map<K, typename std::list<LRUItem>::iterator> m_cache_positions;

public:
  explicit WeightedLRU(int capacity) : m_capacity{capacity}, m_current_cap{} {}

  std::expected<V, std::string> get(K const &key) {
    auto it = m_cache_positions.find(key);
    if (it == m_cache_positions.end()) {
      return std::unexpected{"No such key!"};
    }

    m_store.splice(m_store.begin(), m_store, it->second);
    it->second = m_store.begin();
    return it->second->value;
  }

  void put(K key, V value, int size) {
    if (size > m_capacity) {
      throw std::runtime_error("Sorry this is too big!");
    }

    auto it = m_cache_positions.find(key);
    if (it != m_cache_positions.end()) {
      m_current_cap -= it->second->size;
      m_store.erase(it->second);
      m_cache_positions.erase(it);
    }

    while (m_current_cap + size > m_capacity) {
      const auto &last = m_store.back();
      m_cache_positions.erase(last.key);
      m_current_cap -= last.size;
      m_store.pop_back();
    }

    m_store.push_front({std::move(key), std::move(value), size});
    m_cache_positions[m_store.front().key] = m_store.begin();
    m_current_cap += size;
  }
};

} // namespace ming

#endif // MING_WLRU