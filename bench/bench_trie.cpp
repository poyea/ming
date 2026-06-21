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
