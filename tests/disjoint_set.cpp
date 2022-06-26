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

  ming::DisjointSet<int> dsu_base, dsu;
};

TEST_F(DisjointSet_TEST, DisjointSetInsert) {
  dsu = dsu_base;
  std::vector<ming::DisjointSet<int>::node_ptr> vec{dsu.insert(1), dsu.insert(2),
                                                         dsu.insert(3), dsu.insert(4)};
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

  EXPECT_TRUE(dsu.find(vec[0])->get_object() == 1);
  EXPECT_TRUE(dsu.find(vec[1])->get_object() == 1);
  EXPECT_TRUE(dsu.find(vec[2])->get_object() == 3);
  EXPECT_TRUE(dsu.find(vec[3])->get_object() == 4);
}

TEST_F(DisjointSet_TEST, DisjointSetNode) {
  ming::DisjointSet<int>::node_ptr it = dsu.insert(5), it2 = dsu.insert(6),
                                        it3 = dsu.insert(7);
  dsu.merge(it, it2);
  dsu.merge(it, it3);
  ming::DisjointSetNode<int> it_copy(*it2.get());
  EXPECT_TRUE(&(*it2) != &it_copy);
  EXPECT_TRUE(it2->get_rank() == it_copy.get_rank());
  EXPECT_TRUE(&it2->get_object() != &it_copy.get_object());
  EXPECT_TRUE(&it2->get_parent() != &it_copy.get_parent());
  EXPECT_TRUE(it->get_parent().get() == nullptr);

  auto &it2_parent = it2->get_parent().get()->get_object();
  auto &it_copy_parent = it_copy.get_parent().get()->get_object();
  auto &it3_parent = it3->get_parent().get()->get_object();
  EXPECT_TRUE(it2_parent == it_copy_parent);
  EXPECT_TRUE(it3_parent == it_copy_parent);
  EXPECT_TRUE(it2_parent == it3_parent);

  ming::DisjointSetNode<int> it_moved = std::move(*it2.get());
  auto &it_moved_parent = it_moved.get_parent().get()->get_object();
  EXPECT_TRUE(it_moved_parent == it_copy_parent);
  EXPECT_TRUE(it3_parent == it_copy_parent);
  EXPECT_TRUE(it_moved_parent == it3_parent);

  ming::DisjointSetNode<int> it_moved2(std::move(it_moved));
  auto &it_moved2_parent = it_moved2.get_parent().get()->get_object();
  EXPECT_TRUE(it_moved2_parent == it_copy_parent);
  EXPECT_TRUE(it3_parent == it_copy_parent);
  EXPECT_TRUE(it_moved2_parent == it3_parent);
}

} // namespace disjoint_set_test