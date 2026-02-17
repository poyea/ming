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

#include "gtest/gtest.h"

#include <ming/skiplist.hpp>
#include <string>

namespace skiplist_test {

class SkipList_TEST : public ::testing::Test {
protected:
  SkipList_TEST() = default;
  ~SkipList_TEST() override = default;

  void SetUp() override {
    // post-construction
  }

  void TearDown() override {
    // pre-destruction
  }

  ming::SkipList<int, std::string> skiplist;
};

TEST_F(SkipList_TEST, BasicOperations) {
  // Test insert
  EXPECT_TRUE(skiplist.insert(5, "five"));
  EXPECT_TRUE(skiplist.insert(3, "three"));
  EXPECT_TRUE(skiplist.insert(7, "seven"));

  // Test duplicate insert (should fail)
  EXPECT_FALSE(skiplist.insert(5, "five_duplicate"));

  // Test size
  EXPECT_EQ(skiplist.size(), 3u);

  // Test search
  std::string value;
  EXPECT_TRUE(skiplist.search(5, value));
  EXPECT_EQ(value, "five");

  EXPECT_TRUE(skiplist.search(3, value));
  EXPECT_EQ(value, "three");

  EXPECT_TRUE(skiplist.search(7, value));
  EXPECT_EQ(value, "seven");

  EXPECT_FALSE(skiplist.search(10, value));

  // Test contains
  EXPECT_TRUE(skiplist.contains(5));
  EXPECT_FALSE(skiplist.contains(10));

  // Test erase
  EXPECT_TRUE(skiplist.erase(3));
  EXPECT_EQ(skiplist.size(), 2u);
  EXPECT_FALSE(skiplist.search(3, value));

  // Test erase non-existent (should fail)
  EXPECT_FALSE(skiplist.erase(10));

  // Test iterator
  for (auto it = skiplist.begin(); it != skiplist.end(); ++it) {
    auto [key, val] = *it;
    (void)key;
    (void)val;
  }
}

class SkipListReverseComp_TEST : public ::testing::Test {
protected:
  SkipListReverseComp_TEST() = default;
  ~SkipListReverseComp_TEST() override = default;

  void SetUp() override {
    // post-construction
  }

  void TearDown() override {
    // pre-destruction
  }

  static bool reverse_comp(int a, int b) { return a > b; }
  ming::SkipList<int, std::string, decltype(&SkipListReverseComp_TEST::reverse_comp)>
      skiplist{&SkipListReverseComp_TEST::reverse_comp};
};

TEST_F(SkipListReverseComp_TEST, CustomComparator) {
  // Insert data
  EXPECT_TRUE(skiplist.insert(5, "five"));
  EXPECT_TRUE(skiplist.insert(3, "three"));
  EXPECT_TRUE(skiplist.insert(7, "seven"));

  // Test search with custom comparator
  std::string value;
  EXPECT_TRUE(skiplist.search(5, value));
  EXPECT_EQ(value, "five");

  EXPECT_TRUE(skiplist.search(3, value));
  EXPECT_EQ(value, "three");

  EXPECT_TRUE(skiplist.search(7, value));
  EXPECT_EQ(value, "seven");

  // Check iterator order (should be descending)
  int prev = 0;
  bool first = true;
  for (auto it = skiplist.begin(); it != skiplist.end(); ++it) {
    auto [key, val] = *it;
    (void)val;

    if (!first) {
      EXPECT_GE(prev, key);
    } else {
      first = false;
    }
    prev = key;
  }
}

struct ComplexKey {
  int id;
  std::string name;

  bool operator<(ComplexKey const &other) const { return id < other.id; }
};

class SkipListComplexKey_TEST : public ::testing::Test {
protected:
  SkipListComplexKey_TEST() = default;
  ~SkipListComplexKey_TEST() override = default;

  void SetUp() override {
    // post-construction
  }

  void TearDown() override {
    // pre-destruction
  }

  ming::SkipList<ComplexKey, std::string> skiplist;
};

TEST_F(SkipListComplexKey_TEST, ComplexKey) {
  skiplist.insert({1, "one"}, "Value for key 1");
  skiplist.insert({2, "two"}, "Value for key 2");
  skiplist.insert({3, "three"}, "Value for key 3");

  // Test search
  std::string value;
  EXPECT_TRUE(skiplist.search({2, "two"}, value));
  EXPECT_EQ(value, "Value for key 2");

  // Test searching for non-existent key
  EXPECT_FALSE(skiplist.search({4, "four"}, value));

  // Test erase
  EXPECT_TRUE(skiplist.erase({1, "one"}));
  EXPECT_FALSE(skiplist.search({1, "one"}, value));
  EXPECT_EQ(skiplist.size(), 2u);
}

} // namespace skiplist_test
