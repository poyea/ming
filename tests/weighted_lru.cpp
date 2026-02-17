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

#include <ming/weighted_lru.hpp>

namespace weighted_lru_test {

class WeightedLRU_TEST : public ::testing::Test {
protected:
  ming::WeightedLRU<std::string, int> weighted_lru{10};
};

TEST_F(WeightedLRU_TEST, GetMissingKeyReturnsError) {
  auto r = weighted_lru.get("missing");
  EXPECT_FALSE(r.has_value()) << "missing key should return unexpected";
  EXPECT_EQ(r.error(), "No such key!") << "error string should match";
}

TEST_F(WeightedLRU_TEST, GetExistingKeyReturnsValue) {
  weighted_lru.put("a", 42, 5);

  auto r = weighted_lru.get("a");
  ASSERT_TRUE(r.has_value()) << "existing key should be present";
  EXPECT_EQ(r.value(), 42) << "get existing key returns value";
}

TEST_F(WeightedLRU_TEST, PutUpdateOverwritesValue) {
  weighted_lru.put("a", 1, 5);
  weighted_lru.put("a", 2, 3);

  auto r = weighted_lru.get("a");
  ASSERT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), 2) << "put update overwrites value";
}

TEST_F(WeightedLRU_TEST, TwoItemsFitExactlyAtCapacity) {
  weighted_lru.put("a", 1, 5);
  weighted_lru.put("b", 2, 5);

  auto ra = weighted_lru.get("a");
  auto rb = weighted_lru.get("b");
  ASSERT_TRUE(ra.has_value());
  ASSERT_TRUE(rb.has_value());
  EXPECT_EQ(ra.value(), 1);
  EXPECT_EQ(rb.value(), 2);
}

TEST_F(WeightedLRU_TEST, LRUItemEvictedWhenCapacityExceeded) {
  weighted_lru.put("a", 1, 6);
  weighted_lru.put("b", 2, 6); // evict "a"

  auto ra = weighted_lru.get("a");
  EXPECT_FALSE(ra.has_value()) << "a should be evicted";
  EXPECT_EQ(ra.error(), "No such key!");

  auto rb = weighted_lru.get("b");
  ASSERT_TRUE(rb.has_value());
  EXPECT_EQ(rb.value(), 2);
}

TEST_F(WeightedLRU_TEST, MultipleEvictions) {
  weighted_lru.put("a", 1, 3);
  weighted_lru.put("b", 2, 3);
  weighted_lru.put("c", 3, 3); // total=9
  weighted_lru.put("d", 4, 5); // evict a + b

  EXPECT_FALSE(weighted_lru.get("a").has_value());
  EXPECT_FALSE(weighted_lru.get("b").has_value());

  auto rc = weighted_lru.get("c");
  auto rd = weighted_lru.get("d");
  ASSERT_TRUE(rc.has_value());
  ASSERT_TRUE(rd.has_value());
  EXPECT_EQ(rc.value(), 3);
  EXPECT_EQ(rd.value(), 4);
}

TEST_F(WeightedLRU_TEST, LRUEvictionOrderAfterAccess) {
  weighted_lru.put("a", 1, 4);
  weighted_lru.put("b", 2, 4);

  (void)weighted_lru.get("a"); // touch a, so b is LRU
  weighted_lru.put("c", 3, 4); // evict b

  EXPECT_FALSE(weighted_lru.get("b").has_value());

  auto ra = weighted_lru.get("a");
  auto rc = weighted_lru.get("c");
  ASSERT_TRUE(ra.has_value());
  ASSERT_TRUE(rc.has_value());
  EXPECT_EQ(ra.value(), 1);
  EXPECT_EQ(rc.value(), 3);
}

TEST_F(WeightedLRU_TEST, PutUpdatePromotesToMRU) {
  weighted_lru.put("a", 1, 4);
  weighted_lru.put("b", 2, 4);

  weighted_lru.put("a", 10, 4); // promote a to MRU
  weighted_lru.put("c", 3, 4);  // evict b

  EXPECT_FALSE(weighted_lru.get("b").has_value());

  auto ra = weighted_lru.get("a");
  ASSERT_TRUE(ra.has_value());
  EXPECT_EQ(ra.value(), 10);
}

TEST_F(WeightedLRU_TEST, UpdateShrinksWeight) {
  weighted_lru.put("a", 1, 8);
  weighted_lru.put("a", 2, 3);
  weighted_lru.put("b", 3, 7); // fits now

  auto ra = weighted_lru.get("a");
  auto rb = weighted_lru.get("b");
  ASSERT_TRUE(ra.has_value());
  ASSERT_TRUE(rb.has_value());
  EXPECT_EQ(ra.value(), 2);
  EXPECT_EQ(rb.value(), 3);
}

TEST_F(WeightedLRU_TEST, SingleItemFillsEntireCapacity) {
  weighted_lru.put("big", 99, 10);

  auto rbig = weighted_lru.get("big");
  ASSERT_TRUE(rbig.has_value());
  EXPECT_EQ(rbig.value(), 99);

  weighted_lru.put("new", 1, 3); // evict big

  EXPECT_FALSE(weighted_lru.get("big").has_value());

  auto rnew = weighted_lru.get("new");
  ASSERT_TRUE(rnew.has_value());
  EXPECT_EQ(rnew.value(), 1);
}

TEST_F(WeightedLRU_TEST, PutWithSizeGreaterThanCapacityThrows) {
  EXPECT_THROW(weighted_lru.put("huge", 1, 11), std::runtime_error);
}

TEST_F(WeightedLRU_TEST, UpdateGrowingWeightEvictsLRU) {
  weighted_lru.put("a", 1, 6);
  weighted_lru.put("b", 2, 4);

  weighted_lru.put("a", 10, 8); // must evict b

  auto ra = weighted_lru.get("a");
  ASSERT_TRUE(ra.has_value());
  EXPECT_EQ(ra.value(), 10);

  EXPECT_FALSE(weighted_lru.get("b").has_value());
}

TEST_F(WeightedLRU_TEST, UpdateGrowingWeightMayEvictMultipleLRUs) {
  weighted_lru.put("a", 1, 3);
  weighted_lru.put("b", 2, 3);
  weighted_lru.put("c", 3, 3);

  weighted_lru.put("a", 100, 8); // evict b then c

  auto ra = weighted_lru.get("a");
  ASSERT_TRUE(ra.has_value());
  EXPECT_EQ(ra.value(), 100);

  EXPECT_FALSE(weighted_lru.get("b").has_value());
  EXPECT_FALSE(weighted_lru.get("c").has_value());
}

TEST(WeightedLRU, CapacityOneCache) {
  ming::WeightedLRU<std::string, int> lru{1};

  lru.put("a", 1, 1);
  auto ra = lru.get("a");
  ASSERT_TRUE(ra.has_value());
  EXPECT_EQ(ra.value(), 1);

  lru.put("b", 2, 1); // evict a
  EXPECT_FALSE(lru.get("a").has_value());

  auto rb = lru.get("b");
  ASSERT_TRUE(rb.has_value());
  EXPECT_EQ(rb.value(), 2);
}

} // namespace weighted_lru_test
