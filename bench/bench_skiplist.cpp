#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include <benchmark/benchmark.h>
#include <ming/skiplist.hpp>

static std::size_t constexpr const N = 100000;

static void BM_SkipListInsert(benchmark::State &state) {
  ming::SkipList<size_t, size_t> sl;

  for (auto _ : state) {
    for (size_t i = 0; i < N; ++i) {
      sl.insert(i, i * 10);
    }
    benchmark::DoNotOptimize(sl);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_SkipListInsert);

static void BM_SkipListSearch(benchmark::State &state) {
  ming::SkipList<size_t, size_t> sl;
  for (size_t i = 0; i < N; ++i) {
    sl.insert(i, i * 10);
  }

  std::mt19937_64 gen(123);
  std::uniform_int_distribution<size_t> dist(0, N - 1);

  for (auto _ : state) {
    for (size_t i = 0; i < N; ++i) {
      size_t key = dist(gen);
      size_t val = 0;
      auto found = sl.search(key, val);
    }
    benchmark::DoNotOptimize(sl);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_SkipListSearch);

static void BM_SkipListErase(benchmark::State &state) {
  ming::SkipList<size_t, size_t> sl;
  for (size_t i = 0; i < N; ++i) {
    sl.insert(i, i * 10);
  }

  for (auto _ : state) {
    for (size_t i = 0; i < N / 2; ++i) {
      sl.erase(i);
    }
    benchmark::DoNotOptimize(sl);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_SkipListErase);

BENCHMARK_MAIN();
