#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <ming/trie.hpp>

using Clock = std::chrono::steady_clock;

static std::string mkstr(size_t i) {
  // simple deterministic string generator
  std::string s = "w" + std::to_string(i) + "_bench";
  return s;
}

int main(int argc, char **argv) {
  size_t N = 100000;
  if (argc > 1)
    N = std::stoul(argv[1]);

  ming::Trie<char> trie;
  std::vector<std::string> keys;
  keys.reserve(N);
  for (size_t i = 0; i < N; ++i)
    keys.push_back(mkstr(i));

  auto t0 = Clock::now();
  for (size_t i = 0; i < N; ++i)
    trie.insert(keys[i]);
  auto t1 = Clock::now();

  std::mt19937_64 gen(7);
  std::uniform_int_distribution<size_t> dist(0, N - 1);
  size_t found = 0;
  auto t2 = Clock::now();
  for (size_t i = 0; i < N; ++i) {
    if (trie.is_word(keys[dist(gen)]))
      ++found;
  }
  auto t3 = Clock::now();

  auto ms = [](auto d) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
  };

  std::cout << "Trie N=" << N << "\n";
  std::cout << " insert: " << ms(t1 - t0) << " ms\n";
  std::cout << " is_word: " << ms(t3 - t2) << " ms (found=" << found << ")\n";

  return 0;
}
