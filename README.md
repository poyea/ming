# ming

C++ containers library

## Build Instructions

*Note: Please use a modern `clang`.*

Setting `CC` and `CXX`:

```
export CC=/usr/bin/clang-22
export CXX=/usr/bin/clang++-22
```

Build:

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