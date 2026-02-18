#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include <ming/skiplist.hpp>

using Clock = std::chrono::steady_clock;

int main(int argc, char **argv) {
  size_t N = 100000;
  if (argc > 1)
    N = std::stoul(argv[1]);

  ming::SkipList<size_t, size_t> sl;

  auto t0 = Clock::now();
  for (size_t i = 0; i < N; ++i) {
    sl.insert(i, i * 10);
  }
  auto t1 = Clock::now();

  std::mt19937_64 gen(123);
  std::uniform_int_distribution<size_t> dist(0, N - 1);

  size_t found = 0;
  auto t2 = Clock::now();
  for (size_t i = 0; i < N; ++i) {
    size_t key = dist(gen);
    size_t val = 0;
    if (sl.search(key, val))
      ++found;
  }
  auto t3 = Clock::now();

  // erase half
  auto t4 = Clock::now();
  for (size_t i = 0; i < N / 2; ++i) {
    sl.erase(i);
  }
  auto t5 = Clock::now();

  auto ms = [](auto d) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
  };

  std::cout << "SkipList N=" << N << "\n";
  std::cout << " insert: " << ms(t1 - t0) << " ms\n";
  std::cout << " search: " << ms(t3 - t2) << " ms (found=" << found << ")\n";
  std::cout << " erase:  " << ms(t5 - t4) << " ms\n";

  return 0;
}
