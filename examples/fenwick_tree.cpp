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

#include <ming/fenwick_tree.hpp>

#include <array>
#include <iostream>
#include <span>

int main() {
  std::vector<std::int64_t> data{1, 2, 3, 4, 5, 6, 7, 8};
  ming::FenwickTree t(data);

  if (auto r = t.sum(0, 7); r) {
    std::cout << "sum[0..7] = " << *r << '\n'; // 36
  }

  if (auto r = t.add(3, 10); !r) {
    std::cout << "add failed: " << r.error().message << '\n';
  }

  if (auto r = t.sum(2, 4); r) {
    std::cout << "sum[2..4] = " << *r << '\n'; // 3 + 14 + 5 = 22
  }

  if (auto r = t.add(100, 1); !r) {
    std::cout << "checked error: " << r.error().message << '\n';
  }
}
