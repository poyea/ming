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

#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>
#include <ming/disjoint_set.hpp>

static std::size_t N = 100000;

static void BM_DisjointSetPush(benchmark::State &state) {
  ming::DisjointSet<int> dsu;
  std::vector<ming::DisjointSet<int>::Iterator> nodes;
  nodes.reserve(N);

  for (auto _ : state) {
    for (std::size_t i = 0; i < N; ++i) {
      nodes.push_back(dsu.insert(static_cast<int>(i)));
    };

    benchmark::DoNotOptimize(dsu);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_DisjointSetPush);

static void BM_DisjointSetMerge(benchmark::State &state) {
  ming::DisjointSet<int> dsu;
  std::vector<ming::DisjointSet<int>::Iterator> nodes;
  nodes.reserve(N);

  for (std::size_t i = 0; i < N; ++i) {
    nodes.push_back(dsu.insert(static_cast<int>(i)));
  };

  std::mt19937_64 gen(42);
  std::uniform_int_distribution<std::size_t> dist(0, N - 1);

  for (auto _ : state) {
    for (std::size_t i = 0; i < N / 2; ++i) {
      std::size_t a = dist(gen);
      std::size_t b = dist(gen);
      dsu.merge(nodes[a], nodes[b]);
    }

    benchmark::DoNotOptimize(dsu);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_DisjointSetMerge);

static void BM_DisjointSetFind(benchmark::State &state) {
  ming::DisjointSet<int> dsu;
  std::vector<ming::DisjointSet<int>::Iterator> nodes;
  nodes.reserve(N);

  for (std::size_t i = 0; i < N; ++i) {
    nodes.push_back(dsu.insert(static_cast<int>(i)));
  };

  std::mt19937_64 gen(42);
  std::uniform_int_distribution<std::size_t> dist(0, N - 1);

  std::size_t same_count = 0;

  for (auto _ : state) {
    for (std::size_t i = 0; i < N; ++i) {
      std::size_t a = dist(gen);
      std::size_t b = dist(gen);
      if (dsu.are_same_set(nodes[a], nodes[b]))
        ++same_count;
    }

    benchmark::DoNotOptimize(dsu);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_DisjointSetFind);

BENCHMARK_MAIN();