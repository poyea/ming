#include <ming/trie.hpp>

#include <iostream>

int main(void) {
  ming::Trie t{"race", "racing", "racecar", "rapid", "playground", "plump"};
  t.insert("stuff");

  std::cout << std::boolalpha;
  std::cout << t.is_word("stuff") << '\n';
  std::cout << t.is_word("boy") << '\n';
  std::cout << t.is_word("playground") << '\n';
  std::cout << t.is_word("spaceship") << '\n';

  std::cout << t.starts_with("stu") << '\n';
  std::cout << t.starts_with("ra") << '\n';
  std::cout << t.starts_with("racecar") << '\n';
  std::cout << t.starts_with("boy") << '\n';
  std::cout << t.starts_with("rapid") << '\n';

  auto t2 = t;
  auto t3 = std::move(t2);
}