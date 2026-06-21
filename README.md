# ming

C++ containers library

## Build Instructions

*Note: Please use a modern `clang`.*

```bash
mkdir -p build && cmake -B build -DTESTS=ON && cmake --build build && ctest --test-dir build
```

Just one:

```bash
mkdir -p build && cmake --build build && ctest --test-dir build -R trie
```

Benchmarks:

```bash
cmake -S . -B build -DBENCHMARKS=ON && cmake --build build --parallel
./build/bench/bench_disjoint_set
```

## License

GNU General Public License v3.0