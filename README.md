# ming
C++ containers library

## Build Instructions

```bash
mkdir -p build && cmake -B build -DTESTS=ON && cmake --build build && ctest --test-dir build
```

Just one:

```bash
mkdir -p build && cmake --build build && ctest --test-dir build -R trie
```

Benchmarks:

```bash
cmake -S . -B build -DBENCHMARKS=ON
cmake --build build --parallel
# run a bench (N=1e5)
./build/bench/bench_disjoint_set 100000
```

## License

GNU General Public License v3.0