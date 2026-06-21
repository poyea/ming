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

#include <random>
#include <string>
#include <vector>

#include <benchmark/benchmark.h>
#include <ming/trie.hpp>

static std::string mkstr(std::size_t i) {
  // simple deterministic string generator
  std::string s = "w" + std::to_string(i) + "_bench";
  return s;
}

static void BM_TrieInsert(benchmark::State &state) {
  std::size_t const N = 100000;

  ming::Trie<char> trie;
  std::vector<std::string> keys;
  keys.reserve(N);

  for (auto _ : state) {
    for (std::size_t i = 0; i < N; ++i) {
      keys.push_back(mkstr(i));
    }
  }

  benchmark::DoNotOptimize(trie);
  benchmark::ClobberMemory();
}

BENCHMARK(BM_TrieInsert);

static void BM_TrieFind(benchmark::State &state) {
  std::size_t const N = 100000;

  ming::Trie<char> trie;
  std::vector<std::string> keys;
  keys.reserve(N);
  for (std::size_t i = 0; i < N; ++i) {
    keys.push_back(mkstr(i));
  }

  std::mt19937_64 gen(77);
  std::uniform_int_distribution<std::size_t> dist(0, N - 1);
  std::size_t found = 0;
  for (auto _ : state) {
    for (auto i = 0uz; i < N; ++i) {
      if (trie.is_word(keys[dist(gen)]))
        ++found;
    }

    benchmark::DoNotOptimize(trie);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_TrieFind);

BENCHMARK_MAIN();
