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

#define SUPPRESS_UNUSED _Pragma("GCC diagnostic ignored \"-Wunused-variable\"")

#include <benchmark/benchmark.h>
#include <ming/skiplist.hpp>

static std::size_t constexpr const N = 100000;

static void BM_SkipListInsert(benchmark::State &state) {
  ming::SkipList<std::size_t, std::size_t> sl;

  for (auto _ : state) {
    for (auto i = 0uz; i < N; ++i) {
      sl.insert(i, i * 10);
    }
    benchmark::DoNotOptimize(sl);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_SkipListInsert);

static void BM_SkipListSearch(benchmark::State &state) {
  ming::SkipList<std::size_t, std::size_t> sl;
  for (auto i = 0uz; i < N; ++i) {
    sl.insert(i, i * 10);
  }

  std::mt19937_64 gen(123);
  std::uniform_int_distribution<std::size_t> dist(0, N - 1);

  for (auto _ : state) {
    for (auto i = 0uz; i < N; ++i) {
      std::size_t key = dist(gen);
      std::size_t val = 0;
      auto found = sl.search(key, val);
    }
    benchmark::DoNotOptimize(sl);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_SkipListSearch);

static void BM_SkipListErase(benchmark::State &state) {
  ming::SkipList<std::size_t, std::size_t> sl;
  for (auto i = 0uz; i < N; ++i) {
    sl.insert(i, i * 10);
  }

  for (auto _ : state) {
    for (auto i = 0uz; i < N / 2; ++i) {
      sl.erase(i);
    }

    benchmark::DoNotOptimize(sl);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_SkipListErase);

BENCHMARK_MAIN();
