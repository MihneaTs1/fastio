// MihneaTs1's FastIO library
// Copyright (c) Mihnea-Teodor Stoica 2025
//
// FastIO: High-performance buffered input for C++
// - Supports fast reading of integral, floating-point, and boolean types
// - Compatible with FILE* streams (default: stdin)
// - Modern C++ (C++17+) features and best practices
// - No additional dependencies except standard headers
//
// Usage:
//   FastIO io;
//   int x = io.nextInt<int>();
//   double y = io.nextInt<double>();
//   bool b = io.nextInt<bool>();
//
#pragma once

#include <cstdio>
#include <cstddef>
#include <type_traits>
#include <cmath>
#include <string>

class FastIO {
public:
    explicit FastIO(FILE* fp_ = stdin) noexcept : fp(fp_) {}
    ~FastIO() = default;

    [[gnu::always_inline]] inline int nextChar() noexcept {
        if (__builtin_expect(!fp, 0)) [[unlikely]] return EOF;
        if (__builtin_expect(index >= bytesRead, 0)) [[unlikely]] {
            bytesRead = static_cast<int>(fread(buf, 1, BUF_SIZE, fp));
            index = 0;
            if (__builtin_expect(bytesRead <= 0, 0)) [[unlikely]] return EOF;
        }
        return static_cast<unsigned char>(buf[index++]);
    }

    // Helper: skip whitespace (C locale, fast)
    [[gnu::always_inline]] inline int skipWhitespace() noexcept {
        int c;
        while (true) {
            c = nextChar();
            if (c == EOF) return EOF;
            if (c > 32) return c; // ASCII: space and below are whitespace
        }
    }

    template <typename T>
    T next() noexcept {
        if constexpr (std::is_integral_v<T> && std::is_signed_v<T> && !std::is_same_v<T, bool>) {
            T x = 0;
            int sign = 1;
            int c = skipWhitespace();
            if (c == EOF) [[unlikely]] return 0;
            while ((c == '-' || c == '+')) {
                if (c == '-') sign = -sign;
                c = nextChar();
            }
            while (c >= '0' && c <= '9') {
                x = x * 10 + (c - '0');
                c = nextChar();
            }
            return x * sign;
        } else if constexpr (std::is_floating_point_v<T>) {
            T x = 0;
            int sign = 1;
            int c = skipWhitespace();
            if (c == EOF) [[unlikely]] return 0;
            while ((c == '-' || c == '+')) {
                if (c == '-') sign = -sign;
                c = nextChar();
            }
            bool found_digit = false;
            while (c >= '0' && c <= '9') {
                found_digit = true;
                x = x * 10 + (c - '0');
                c = nextChar();
            }
            if (c == '.') {
                T frac = 0, base = 0.1;
                c = nextChar();
                while (c >= '0' && c <= '9') {
                    found_digit = true;
                    frac += (c - '0') * base;
                    base *= 0.1;
                    c = nextChar();
                }
                x += frac;
            }
            if ((c == 'e' || c == 'E') && found_digit) {
                int exp_sign = 1, exp = 0;
                c = nextChar();
                if (c == '-') { exp_sign = -1; c = nextChar(); }
                else if (c == '+') { c = nextChar(); }
                while (c >= '0' && c <= '9') {
                    exp = exp * 10 + (c - '0');
                    c = nextChar();
                }
                x *= std::pow(10, exp_sign * exp);
            }
            return x * sign;
        } else if constexpr (std::is_same_v<T, bool>) {
            int c = skipWhitespace();
            if (c == EOF) [[unlikely]] return false;
            if (c == '0') return false;
            if (c == '1') return true;
            if (c == 't' || c == 'T') {
                if ((nextChar() | 32) == 'r' && (nextChar() | 32) == 'u' && (nextChar() | 32) == 'e')
                    return true;
            } else if (c == 'f' || c == 'F') {
                if ((nextChar() | 32) == 'a' && (nextChar() | 32) == 'l' && (nextChar() | 32) == 's' && (nextChar() | 32) == 'e')
                    return false;
            }
            if (c >= '2' && c <= '9') return true;
            return false;
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::string s;
            int c = skipWhitespace();
            if (c == EOF) return s;
            s.reserve(16); // Small string optimization
            while (c != EOF && c > 32) {
                s += static_cast<char>(c);
                c = nextChar();
            }
            return s;
        } else if constexpr (std::is_same_v<T, char>) {
            // char specialization: reads next non-whitespace character
            int c = skipWhitespace();
            if (c == EOF) return 0;
            return static_cast<char>(c);
        } else {
            static_assert(sizeof(T) == 0, "Unsupported type for FastIO::next()");
        }
    }

    // char* overload: reads a word into the provided buffer (like std::cin >> buf)
    // The user must provide the buffer and its size
    void next(char* s, std::size_t n) noexcept {
        if (!s || n == 0) return;
        int c = skipWhitespace();
        std::size_t i = 0;
        while (c != EOF && c > 32 && i + 1 < n) {
            s[i++] = static_cast<char>(c);
            c = nextChar();
        }
        s[i] = '\0';
    }

private:
    FILE* fp = nullptr;
    static constexpr std::size_t BUF_SIZE = 1 << 20;
    int index = 0;
    int bytesRead = 0;
    alignas(64) unsigned char buf[BUF_SIZE]{};
};