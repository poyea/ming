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

#define SUPPRESS_UNUSED _Pragma("GCC diagnostic ignored \"-Wunused-but-set-variable\"")

#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include <benchmark/benchmark.h>
#include <ming/weighted_lru.hpp>

static void BM_WeightedLRU(benchmark::State &state) {
  int capacity = 10000;
  std::size_t N = 100000;

  ming::WeightedLRU<int, std::string> cache(capacity);

  std::mt19937_64 gen(99);
  std::uniform_int_distribution<int> size_dist(1, 100);

  for (auto i = 0uz; i < N; ++i) {
    int sz = size_dist(gen);
    cache.put(static_cast<int>(i), std::to_string(i), sz);
  }

  std::uniform_int_distribution<int> key_dist(0, static_cast<int>(N - 1));
  std::vector<int> sampled_keys(N);
  std::ranges::generate(sampled_keys, [&] { return key_dist(gen); });

  std::size_t hits = 0;
  for (auto _ : state) {
    for (auto i = 0uz; i < N; ++i) {
      auto res = cache.get(sampled_keys[i]);
      if (res) {
        ++hits;
      }
    }

    benchmark::DoNotOptimize(cache);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_WeightedLRU);

BENCHMARK_MAIN();