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