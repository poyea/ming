cmake_minimum_required(VERSION 3.23)

set(BENCH_SOURCES
    bench_disjoint_set.cpp
    bench_skiplist.cpp
    bench_trie.cpp
    bench_weighted_lru.cpp
)

foreach(src IN LISTS BENCH_SOURCES)
    get_filename_component(bname ${src} NAME_WE)
    add_executable(${bname} ${src})
    target_link_libraries(${bname} PRIVATE ming::ming)
    target_compile_features(${bname} PRIVATE cxx_std_23)
endforeach()
