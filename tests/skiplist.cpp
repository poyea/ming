#include <cassert>
#include <iostream>
#include <ming/skiplist.hpp>
#include <string>

namespace skiplist_test {

void test_basic_operations() {
  ming::SkipList<int, std::string> skiplist;

  // Test insert
  assert(skiplist.insert(5, "five"));
  assert(skiplist.insert(3, "three"));
  assert(skiplist.insert(7, "seven"));

  // Test duplicate insert (should fail)
  assert(!skiplist.insert(5, "five_duplicate"));

  // Test size
  assert(skiplist.size() == 3);
  std::cout << "Size: " << skiplist.size() << std::endl;

  // Test search
  std::string value;
  assert(skiplist.search(5, value));
  assert(value == "five");
  std::cout << "Found 5: " << value << std::endl;

  assert(skiplist.search(3, value));
  assert(value == "three");
  std::cout << "Found 3: " << value << std::endl;

  assert(skiplist.search(7, value));
  assert(value == "seven");
  std::cout << "Found 7: " << value << std::endl;

  assert(!skiplist.search(10, value));
  std::cout << "Could not find 10" << std::endl;

  // Test contains
  assert(skiplist.contains(5));
  assert(!skiplist.contains(10));

  // Test erase
  assert(skiplist.erase(3));
  assert(skiplist.size() == 2);
  assert(!skiplist.search(3, value));
  std::cout << "Erased 3, size now: " << skiplist.size() << std::endl;

  // Test erase non-existent (should fail)
  assert(!skiplist.erase(10));
  std::cout << "Failed to erase non-existent key 10" << std::endl;

  // Test iterator
  std::cout << "Iterating through skiplist:" << std::endl;
  for (auto it = skiplist.begin(); it != skiplist.end(); ++it) {
    auto [key, val] = *it;
    std::cout << "Key: " << key << ", Value: " << val << std::endl;
  }
}

void test_custom_comparator() {
  auto reverse_comp = [](int a, int b) { return a > b; };
  ming::SkipList<int, std::string, decltype(reverse_comp)> skiplist(reverse_comp);

  // Insert data
  skiplist.insert(5, "five");
  skiplist.insert(3, "three");
  skiplist.insert(7, "seven");

  // Test search with custom comparator
  std::string value;
  assert(skiplist.search(5, value));
  assert(value == "five");
  std::cout << "Custom comparator - Found 5: " << value << std::endl;

  assert(skiplist.search(3, value));
  assert(value == "three");
  std::cout << "Custom comparator - Found 3: " << value << std::endl;

  assert(skiplist.search(7, value));
  assert(value == "seven");
  std::cout << "Custom comparator - Found 7: " << value << std::endl;

  // Check iterator order (should be descending)
  std::cout << "Custom comparator - Iterating (should be in descending order):"
            << std::endl;
  for (auto it = skiplist.begin(); it != skiplist.end(); ++it) {
    auto [key, val] = *it;
    std::cout << "Key: " << key << ", Value: " << val << std::endl;
  }
}

void test_complex_key() {
  struct ComplexKey {
    int id;
    std::string name;

    bool operator<(const ComplexKey &other) const { return id < other.id; }
  };

  ming::SkipList<ComplexKey, std::string> skiplist;

  skiplist.insert({1, "one"}, "Value for key 1");
  skiplist.insert({2, "two"}, "Value for key 2");
  skiplist.insert({3, "three"}, "Value for key 3");

  // Test search
  std::string value;
  assert(skiplist.search({2, "two"}, value));
  assert(value == "Value for key 2");
  std::cout << "Found complex key {2, \"two\"}: " << value << std::endl;

  // Test searching for non-existent key
  assert(!skiplist.search({4, "four"}, value));
  std::cout << "Could not find complex key {4, \"four\"}" << std::endl;

  // Test erase
  assert(skiplist.erase({1, "one"}));
  assert(!skiplist.search({1, "one"}, value));
  std::cout << "Erased complex key {1, \"one\"}, size now: " << skiplist.size()
            << std::endl;
}
} // namespace skiplist_test

int main() {
  std::cout << "Testing basic operations..." << std::endl;
  skiplist_test::test_basic_operations();
  std::cout << "\nTesting custom comparator..." << std::endl;
  skiplist_test::test_custom_comparator();
  std::cout << "\nTesting complex keys..." << std::endl;
  skiplist_test::test_complex_key();
  std::cout << "\nAll tests passed!" << std::endl;
  return 0;
}
