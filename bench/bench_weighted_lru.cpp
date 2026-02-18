#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include <ming/weighted_lru.hpp>

using Clock = std::chrono::steady_clock;

int main(int argc, char **argv) {
  size_t N = 100000;
  if (argc > 1)
    N = std::stoul(argv[1]);

  // small capacity to force evictions
  int capacity = 10000;
  ming::WeightedLRU<int, std::string> cache(capacity);

  std::mt19937_64 gen(99);
  std::uniform_int_distribution<int> size_dist(1, 100);

  auto t0 = Clock::now();
  for (size_t i = 0; i < N; ++i) {
    int sz = size_dist(gen);
    cache.put(static_cast<int>(i), std::to_string(i), sz);
  }
  auto t1 = Clock::now();

  std::uniform_int_distribution<int> key_dist(0, static_cast<int>(N - 1));
  size_t hits = 0;
  auto t2 = Clock::now();
  for (size_t i = 0; i < N; ++i) {
    auto res = cache.get(key_dist(gen));
    if (res)
      ++hits;
  }
  auto t3 = Clock::now();

  auto ms = [](auto d) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
  };

  std::cout << "WeightedLRU N=" << N << "\n";
  std::cout << " put: " << ms(t1 - t0) << " ms\n";
  std::cout << " get: " << ms(t3 - t2) << " ms (hits=" << hits << ")\n";

  return 0;
}
