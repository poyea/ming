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