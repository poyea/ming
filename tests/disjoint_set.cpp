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
  EXPECT_TRUE(it->get()->lock_parent() == nullptr); // root

  auto it2_parent_sp = it2->get()->lock_parent();
  auto it_copy_parent_sp = it_copy->get()->lock_parent();
  auto it3_parent_sp = it3->get()->lock_parent();
  ASSERT_TRUE(it2_parent_sp);
  ASSERT_TRUE(it_copy_parent_sp);
  ASSERT_TRUE(it3_parent_sp);

  EXPECT_EQ(it2_parent_sp, it_copy_parent_sp);
  EXPECT_EQ(it3_parent_sp, it_copy_parent_sp);

  EXPECT_EQ(it2_parent_sp->get_object(), it3_parent_sp->get_object());

  auto it_moved = std::move(it2);
  auto &it_moved_parent = it_moved->get()->lock_parent()->get_object();
  EXPECT_TRUE(it_moved_parent == it_copy_parent_sp->get_object());
  EXPECT_TRUE(it3_parent_sp == it_copy_parent_sp);
  EXPECT_TRUE(it_moved_parent == it3_parent_sp->get_object());

  auto it_moved2(std::move(it_moved));
  auto &it_moved2_parent = it_moved2->get()->lock_parent()->get_object();
  EXPECT_TRUE(it_moved2_parent == it_copy_parent_sp->get_object());
  EXPECT_TRUE(it3_parent_sp == it_copy_parent_sp);
  EXPECT_TRUE(it_moved2_parent == it3_parent_sp->get_object());
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

TEST_F(DisjointSet_TEST, RootParentIsNull) {
  auto it = dsu.insert(42);
  EXPECT_EQ(it->get()->lock_parent(), nullptr);
}

TEST_F(DisjointSet_TEST, CopyConstructorDoesNotDeepCopyParent) {
  auto a = dsu.insert(100);
  auto b = dsu.insert(200);

  dsu.merge(a, b);

  auto b_copy = b; // copy handle

  auto root = *dsu.find(a);
  ASSERT_TRUE(root);

  auto b_parent = b->get()->lock_parent();
  auto b_copy_parent = b_copy->get()->lock_parent();

  ASSERT_TRUE(b_parent);
  ASSERT_TRUE(b_copy_parent);

  EXPECT_EQ(b_parent, b_copy_parent);
  EXPECT_EQ(b_parent, root);
}

TEST_F(DisjointSet_TEST, MovePreservesParentPointer) {
  auto a = dsu.insert(300);
  auto b = dsu.insert(400);

  dsu.merge(a, b);

  auto parent_before = b->get()->lock_parent();
  ASSERT_TRUE(parent_before);

  auto b_moved = std::move(b);
  ASSERT_NE(b_moved->get(), nullptr);

  auto parent_after = b_moved->get()->lock_parent();
  EXPECT_EQ(parent_before, parent_after);
}

TEST_F(DisjointSet_TEST, FindReturnsRootAndPerformsPathCompression) {
  auto a = dsu.insert(1);
  auto b = dsu.insert(2);
  auto c = dsu.insert(3);

  dsu.merge(a, b);
  dsu.merge(b, c); // chain: c -> b -> a

  // before compression, c's immediate parent should be b
  auto c_parent_before = c->get()->lock_parent();
  ASSERT_TRUE(c_parent_before);
  EXPECT_EQ(c_parent_before, *dsu.find(b));

  // calling are_same_set triggers path compression
  EXPECT_TRUE(dsu.are_same_set(c, a));

  auto c_parent_after = c->get()->lock_parent();
  auto root = *dsu.find(a);
  ASSERT_TRUE(root);
  EXPECT_EQ(c_parent_after, root);
}

} // namespace disjoint_set_test