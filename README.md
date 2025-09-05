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

## License

GNU General Public License v3.0