cmake_minimum_required(VERSION 4.3.2)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.9.5
)
FetchContent_MakeAvailable(benchmark)

set(BENCH_SOURCES
    bench_disjoint_set.cpp
    bench_skiplist.cpp
    bench_trie.cpp
    bench_weighted_lru.cpp
    bench_ring_buffer.cpp
)

foreach(src IN LISTS BENCH_SOURCES)
    get_filename_component(bname ${src} NAME_WE)
    add_executable(${bname} ${src})
    target_link_libraries(${bname} PRIVATE benchmark::benchmark_main)
    target_link_libraries(${bname} PRIVATE ming::ming)
    target_compile_features(${bname} PRIVATE cxx_std_26)
endforeach()
