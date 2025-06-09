# FastIO: High-Performance Buffered Input for C++

FastIO is a modern, header-only C++ library for extremely fast, buffered input from files or standard input. It is designed for competitive programming, data processing, and any scenario where input speed is critical.

## Features

- **Blazing fast**: Uses a large buffer and minimal parsing overhead for maximum throughput.
- **Type-safe**: Supports reading `int`, `long long`, `double`, `bool`, `std::string`, `char`, and C-style strings (`char*`) out of the box.
- **Flexible**: Works with any `FILE*` stream (default: `stdin`).
- **Modern C++**: Written in C++17, using best practices and zero dependencies beyond the standard library.
- **Simple API**: One header, one class, one function to read any supported type.

## Usage

Include the header and use the `FastIO` class:

```cpp
#include "fastio.hpp"

FastIO io;
int x = io.next<int>();
double y = io.next<double>();
bool b = io.next<bool>();
std::string s = io.next<std::string>();
char c = io.next<char>();
char buf[32];
io.next(buf, sizeof(buf)); // Reads a word into buf (C-style string)
```

You can also use a custom file pointer:

```cpp
FILE* f = fopen("input.txt", "rb");
FastIO io(f);
// ...
fclose(f);
```

## Benchmark

A benchmark is provided in `benchmark.cpp` to compare FastIO's performance for various types:

- Generates large datasets for `int`, `long long`, `double`, `bool`, `std::string`, `char`, and C-style strings (`char*`).
- Measures throughput (MB/s) and total time to read all values.

### Build and Run with Makefile

A `Makefile` is provided for convenience:

```sh
make        # Builds the benchmark executable
make run    # Builds and runs the benchmark
make clean  # Removes the executable and generated data files
```

### Manual Build

You can also build manually:

```sh
g++ -Ofast -std=c++17 benchmark.cpp -o benchmark
./benchmark
```

## License

Copyright (c) Mihnea-Teodor Stoica 2025

This project is licensed under the MIT License. See the source file headers for details.
