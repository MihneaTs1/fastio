#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <string>
#include <type_traits>
#include <sys/stat.h>
#include "fastio.hpp"  // include your FastIO class here

template<typename T>
void generate_data(const std::string& filename, size_t count) {
    std::ofstream fout(filename);
    std::mt19937_64 rng(42); // Fixed seed for reproducibility

    if constexpr (std::is_same_v<T, int>) {
        std::uniform_int_distribution<int> dist(-1e6, 1e6);
        for (size_t i = 0; i < count; ++i)
            fout << dist(rng) << ' ';
    } else if constexpr (std::is_same_v<T, long long>) {
        std::uniform_int_distribution<long long> dist(-1e12, 1e12);
        for (size_t i = 0; i < count; ++i)
            fout << dist(rng) << ' ';
    } else if constexpr (std::is_same_v<T, double>) {
        std::uniform_real_distribution<double> dist(-1e5, 1e5);
        for (size_t i = 0; i < count; ++i)
            fout << dist(rng) << ' ';
    } else if constexpr (std::is_same_v<T, bool>) {
        std::uniform_int_distribution<int> dist(0, 1);
        for (size_t i = 0; i < count; ++i)
            fout << (dist(rng) ? "true" : "false") << ' ';
    }
}

template<>
void generate_data<std::string>(const std::string& filename, size_t count) {
    std::ofstream fout(filename);
    std::mt19937_64 rng(42);
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<int> len_dist(1, 20);
    std::uniform_int_distribution<int> char_dist(0, sizeof(charset) - 2);
    for (size_t i = 0; i < count; ++i) {
        int len = len_dist(rng);
        for (int j = 0; j < len; ++j)
            fout << charset[char_dist(rng)];
        fout << ' ';
    }
}

void benchmark_str(size_t count) {
    const std::string filename = "str_data.txt";
    generate_data<std::string>(filename, count);

    FILE* f = std::fopen(filename.c_str(), "rb");
    if (!f) {
        std::cerr << "Failed to open " << filename << "\n";
        return;
    }

    FastIO io(f);
    std::size_t read_count = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        int peek = io.nextChar();
        if (peek == EOF) break;
        ++read_count;
        (void)io.next<std::string>();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::fclose(f);

    struct stat st;
    stat(filename.c_str(), &st);
    double size_in_bytes = static_cast<double>(st.st_size);
    double time_sec = std::chrono::duration<double>(end - start).count();
    double throughput = size_in_bytes / time_sec;

    std::cout << "string: "
              << throughput / (1024.0 * 1024.0) << " MB/s, "
              << read_count << " values in "
              << time_sec << " s\n";

    std::remove(filename.c_str());
}

template<typename T>
void benchmark_type(const std::string& typename_str, size_t count) {
    const std::string filename = typename_str + "_data.txt";
    generate_data<T>(filename, count);

    FILE* f = std::fopen(filename.c_str(), "rb");
    if (!f) {
        std::cerr << "Failed to open " << filename << "\n";
        return;
    }

    FastIO io(f);
    std::size_t read_count = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        int peek = io.nextChar();
        if (peek == EOF) break;
        ++read_count;
        (void)io.next<T>();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::fclose(f);

    struct stat st;
    stat(filename.c_str(), &st);
    double size_in_bytes = static_cast<double>(st.st_size);
    double time_sec = std::chrono::duration<double>(end - start).count();
    double throughput = size_in_bytes / time_sec;

    std::cout << typename_str << ": "
              << throughput / (1024.0 * 1024.0) << " MB/s, "
              << read_count << " values in "
              << time_sec << " s\n";

    std::remove(filename.c_str()); // Clean up
}

void benchmark_char(size_t count) {
    const std::string filename = "char_data.txt";
    // Generate data: random visible ASCII characters
    std::ofstream fout(filename);
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int> dist(33, 126); // printable ASCII
    for (size_t i = 0; i < count; ++i) fout << static_cast<char>(dist(rng)) << ' ';
    fout.close();

    FILE* f = std::fopen(filename.c_str(), "rb");
    if (!f) {
        std::cerr << "Failed to open " << filename << "\n";
        return;
    }
    FastIO io(f);
    std::size_t read_count = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        int peek = io.nextChar();
        if (peek == EOF) break;
        ++read_count;
        (void)io.next<char>();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::fclose(f);

    struct stat st;
    stat(filename.c_str(), &st);
    double size_in_bytes = static_cast<double>(st.st_size);
    double time_sec = std::chrono::duration<double>(end - start).count();
    double throughput = size_in_bytes / time_sec;

    std::cout << "char: "
              << throughput / (1024.0 * 1024.0) << " MB/s, "
              << read_count << " values in "
              << time_sec << " s\n";
    std::remove(filename.c_str());
}

void benchmark_cstr(size_t count) {
    const std::string filename = "cstr_data.txt";
    // Generate data: random words (length 1-20)
    std::ofstream fout(filename);
    std::mt19937_64 rng(42);
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<int> len_dist(1, 20);
    std::uniform_int_distribution<int> char_dist(0, sizeof(charset) - 2);
    for (size_t i = 0; i < count; ++i) {
        int len = len_dist(rng);
        for (int j = 0; j < len; ++j)
            fout << charset[char_dist(rng)];
        fout << ' ';
    }
    fout.close();

    FILE* f = std::fopen(filename.c_str(), "rb");
    if (!f) {
        std::cerr << "Failed to open " << filename << "\n";
        return;
    }
    FastIO io(f);
    std::size_t read_count = 0;
    constexpr std::size_t BUF_SIZE = 32;
    char buf[BUF_SIZE];
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        int peek = io.nextChar();
        if (peek == EOF) break;
        ++read_count;
        io.next(buf, BUF_SIZE);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::fclose(f);

    struct stat st;
    stat(filename.c_str(), &st);
    double size_in_bytes = static_cast<double>(st.st_size);
    double time_sec = std::chrono::duration<double>(end - start).count();
    double throughput = size_in_bytes / time_sec;

    std::cout << "cstr: "
              << throughput / (1024.0 * 1024.0) << " MB/s, "
              << read_count << " values in "
              << time_sec << " s\n";
    std::remove(filename.c_str());
}

int main() {
    constexpr size_t N = 100'000'000; // Number of values to generate and read
    benchmark_type<int>("int", N);
    benchmark_type<long long>("ll", N);
    benchmark_type<double>("double", N);
    benchmark_type<bool>("bool", N);
    benchmark_str(N);
    benchmark_char(N);
    benchmark_cstr(N);
    return 0;
}
