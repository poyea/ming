#include <iostream>
#include <ming/trie.hpp>

int main() {
  ming::Trie t;
  t.insert("race");
  t.insert("racing");
  t.insert("racecar");
  t.insert("rapid");
  t.insert("playground");
  t.insert("plump");
  t.insert("stuff");

  std::cout << std::boolalpha;
  std::cout << t.search("stuff") << '\n';
  std::cout << t.search("boy") << '\n';
  std::cout << t.search("playground") << '\n';
  std::cout << t.search("spaceship") << '\n';

  std::cout << t.starts_with("stu") << '\n';
  std::cout << t.starts_with("ra") << '\n';
  std::cout << t.starts_with("racecar") << '\n';
  std::cout << t.starts_with("boy") << '\n';
  std::cout << t.starts_with("rapid") << '\n';

  auto t2 = t;
  auto t3 = std::move(t2);
}