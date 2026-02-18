#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include <ming/disjoint_set.hpp>

using Clock = std::chrono::steady_clock;

int main(int argc, char **argv) {
  size_t N = 100000;
  if (argc > 1)
    N = std::stoul(argv[1]);

  ming::DisjointSet<int> dsu;
  std::vector<ming::DisjointSet<int>::Iterator> nodes;
  nodes.reserve(N);

  auto t0 = Clock::now();
  for (size_t i = 0; i < N; ++i) {
    nodes.push_back(dsu.insert(static_cast<int>(i)));
  }
  auto t1 = Clock::now();

  std::mt19937_64 gen(42);
  std::uniform_int_distribution<size_t> dist(0, N - 1);

  // random merges
  auto t2 = Clock::now();
  for (size_t i = 0; i < N / 2; ++i) {
    size_t a = dist(gen);
    size_t b = dist(gen);
    dsu.merge(nodes[a], nodes[b]);
  }
  auto t3 = Clock::now();

  // repeated finds
  size_t same_count = 0;
  auto t4 = Clock::now();
  for (size_t i = 0; i < N; ++i) {
    size_t a = dist(gen);
    size_t b = dist(gen);
    if (dsu.are_same_set(nodes[a], nodes[b]))
      ++same_count;
  }
  auto t5 = Clock::now();

  auto ms = [](auto d) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
  };

  std::cout << "DisjointSet N=" << N << "\n";
  std::cout << " insert: " << ms(t1 - t0) << " ms\n";
  std::cout << " merge:  " << ms(t3 - t2) << " ms\n";
  std::cout << " are_same_set checks: " << ms(t5 - t4) << " ms (" << same_count
            << ")\n";

  return 0;
}
