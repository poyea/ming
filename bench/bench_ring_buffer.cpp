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

#include <benchmark/benchmark.h>
#include <ming/ring_buffer.hpp>

static void BM_RingBufferPush10(benchmark::State &state) {
  ming::RingBuffer<int> rb{10};
  int item = 117;

  for (auto _ : state) {
    rb.push(item);

    benchmark::DoNotOptimize(rb);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_RingBufferPush10);

static void BM_RingBufferPush1000(benchmark::State &state) {
  ming::RingBuffer<int> rb{1000};
  int item = 117;

  for (auto _ : state) {
    rb.push(item);

    benchmark::DoNotOptimize(rb);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_RingBufferPush1000);

BENCHMARK_MAIN();