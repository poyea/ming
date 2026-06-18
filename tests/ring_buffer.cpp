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

#include <ming/ring_buffer.hpp>
#include <string>

namespace ring_buffer_test {

class RingBuffer_TEST : public ::testing::Test {
protected:
  RingBuffer_TEST() = default;
  ~RingBuffer_TEST() override = default;

  void SetUp() override {
    // post-construction
  }

  void TearDown() override {
    // pre-destruction
  }

  ming::RingBuffer<int> ring_buffer{5};
};

TEST_F(RingBuffer_TEST, BasicOperations) {
  ring_buffer.push(1);
  ring_buffer.push(2);
  ring_buffer.push(3);
  ring_buffer.push(4);
  ring_buffer.push(5);

  EXPECT_EQ(ring_buffer.size(), 5u);

  ring_buffer.push(6);

  EXPECT_EQ(ring_buffer.size(), 5u);
}

} // namespace ring_buffer_test
