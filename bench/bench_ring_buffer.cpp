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