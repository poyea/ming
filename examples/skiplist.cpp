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

#include <ming/skiplist.hpp>

#include <iostream>
#include <string>

int main(void) {
  ming::SkipList<int, std::string> skiplist;

  skiplist.insert(3, "Three");
  skiplist.insert(1, "One");
  skiplist.insert(7, "Seven");
  skiplist.insert(8, "Eight");
  skiplist.insert(9, "Nine");
  skiplist.insert(12, "Twelve");
  skiplist.insert(10, "Ten");

  std::cout << "Skip list size: " << skiplist.size() << std::endl;

  // searching
  std::string result;
  std::cout << std::boolalpha;

  std::cout << "Search for key 7: " << skiplist.search(7, result);
  std::cout << ", value: " << result << std::endl;

  std::cout << "Search for key 5: " << skiplist.search(5, result) << std::endl;

  std::cout << "Contains key 10: " << skiplist.contains(10) << std::endl;
  std::cout << "Contains key 20: " << skiplist.contains(20) << std::endl;

  // erasure
  std::cout << "Erase key 9: " << skiplist.erase(9) << std::endl;
  std::cout << "Size after erasure: " << skiplist.size() << std::endl;
  std::cout << "Contains key 9 after erasure: " << skiplist.contains(9) << std::endl;

  // iterator
  std::cout << "\nIterating through the skiplist:" << std::endl;
  for (auto it = skiplist.begin(); it != skiplist.end(); ++it) {
    auto [key, value] = *it;
    std::cout << "Key: " << key << ", Value: " << value << std::endl;
  }

  return 0;
}
