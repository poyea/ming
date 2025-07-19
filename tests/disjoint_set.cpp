//
// ming   C++ containers library
// Copyright (C) 2022-2025 John Law
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

#include <ming/disjoint_set.hpp>

namespace disjoint_set_test {

class DisjointSet_TEST : public ::testing::Test {
protected:
  DisjointSet_TEST() = default;

  virtual ~DisjointSet_TEST() {}

  virtual void SetUp() {
    // post-construction
  }

  virtual void TearDown() {
    // pre-destruction
  }

  ming::DisjointSet<int> dsu;
};

TEST_F(DisjointSet_TEST, DisjointSetInsert) {
  std::vector<ming::DisjointSet<int>::Iterator> vec;

  auto it1 = dsu.insert(1), it2 = dsu.insert(2), it3 = dsu.insert(3),
       it4 = dsu.insert(4);
  vec.push_back(it1);
  vec.push_back(it2);
  vec.push_back(it3);
  vec.push_back(it4);

  dsu.merge(vec[0], vec[1]);

  EXPECT_TRUE(dsu.are_same_set(vec[0], vec[1]));
  EXPECT_TRUE(dsu.are_same_set(vec[1], vec[0]));
  EXPECT_TRUE(dsu.are_same_set(vec[1], vec[1]));
  EXPECT_TRUE(dsu.are_same_set(vec[0], vec[0]));
  EXPECT_TRUE(dsu.are_same_set(vec[2], vec[2]));
  EXPECT_TRUE(dsu.are_same_set(vec[3], vec[3]));
  EXPECT_FALSE(dsu.are_same_set(vec[0], vec[2]));
  EXPECT_FALSE(dsu.are_same_set(vec[0], vec[3]));
  EXPECT_FALSE(dsu.are_same_set(vec[3], vec[0]));
  EXPECT_FALSE(dsu.are_same_set(vec[2], vec[0]));

  EXPECT_TRUE(dsu.find(vec[0])->get()->get_object() == 1);
  EXPECT_TRUE(dsu.find(vec[1])->get()->get_object() == 1);
  EXPECT_TRUE(dsu.find(vec[2])->get()->get_object() == 3);
  EXPECT_TRUE(dsu.find(vec[3])->get()->get_object() == 4);
}

TEST_F(DisjointSet_TEST, DisjointSetNode) {
  auto it = dsu.insert(5), it2 = dsu.insert(6), it3 = dsu.insert(7);

  dsu.merge(it, it2);

  dsu.merge(it, it3);

  auto it_copy(it2);
  EXPECT_TRUE(&it2 != &it_copy);
  EXPECT_TRUE(it2->get()->get_rank() == it_copy->get()->get_rank());
  EXPECT_TRUE(&it2->get()->get_object() == &it_copy->get()->get_object());
  EXPECT_TRUE(&it2->get()->get_parent() == &it_copy->get()->get_parent());
  EXPECT_TRUE(it->get()->get_parent().get() == nullptr); // root

  auto &it2_parent = it2->get()->get_parent().get()->get_object();
  auto &it_copy_parent = it_copy->get()->get_parent().get()->get_object();
  auto &it3_parent = it3->get()->get_parent().get()->get_object();
  EXPECT_TRUE(it2_parent == it_copy_parent);
  EXPECT_TRUE(it3_parent == it_copy_parent);
  EXPECT_TRUE(it2_parent == it3_parent);

  auto it_moved = std::move(it2);
  auto &it_moved_parent = it_moved->get()->get_parent().get()->get_object();
  EXPECT_TRUE(it_moved_parent == it_copy_parent);
  EXPECT_TRUE(it3_parent == it_copy_parent);
  EXPECT_TRUE(it_moved_parent == it3_parent);

  auto it_moved2(std::move(it_moved));
  auto &it_moved2_parent = it_moved2->get()->get_parent().get()->get_object();
  EXPECT_TRUE(it_moved2_parent == it_copy_parent);
  EXPECT_TRUE(it3_parent == it_copy_parent);
  EXPECT_TRUE(it_moved2_parent == it3_parent);
}

TEST_F(DisjointSet_TEST, ComplexMergeOperations) {
  std::vector<ming::DisjointSet<int>::Iterator> elements;

  for (int i = 0; i < 10; i++) {
    elements.push_back(dsu.insert(i));
  }

  dsu.merge(elements[0], elements[1]);
  dsu.merge(elements[1], elements[2]);
  dsu.merge(elements[3], elements[4]);
  dsu.merge(elements[5], elements[6]);
  dsu.merge(elements[6], elements[7]);

  EXPECT_TRUE(dsu.are_same_set(elements[0], elements[2]));
  EXPECT_TRUE(dsu.are_same_set(elements[3], elements[4]));
  EXPECT_TRUE(dsu.are_same_set(elements[5], elements[7]));

  EXPECT_FALSE(dsu.are_same_set(elements[0], elements[3]));
  EXPECT_FALSE(dsu.are_same_set(elements[3], elements[5]));
  EXPECT_FALSE(dsu.are_same_set(elements[0], elements[8]));

  dsu.merge(elements[0], elements[3]);
  dsu.merge(elements[5], elements[8]);

  dsu.merge(elements[0], elements[5]);
  dsu.merge(elements[8], elements[9]);

  for (size_t i = 0; i < elements.size(); i++) {
    for (size_t j = i + 1; j < elements.size(); j++) {
      EXPECT_TRUE(dsu.are_same_set(elements[i], elements[j]));
    }
  }
}

} // namespace disjoint_set_test