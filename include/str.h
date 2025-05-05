#pragma once

#include "types.h" // isize
#include <cassert>

struct Str {
    // Hack for holding string literals as well
    union {
        char*       buf = 0;
        const char* cbuf;
    };
    isize len = 0;

    // Default constructor
    Str() = default;

    // From string literals
    template <isize N>
    constexpr Str(const char (&s)[N])
        : cbuf{s},
          len{N - 1}
    {
    }

    // From spans
    Str(char* beg, char* end)
    {
        assert(beg <= end);
        buf = beg;
        len = end - beg;
    }

    // str_equal
    b32 operator==(Str s) { return len == s.len && (!len || !memcmp(buf, s.buf, len)); }

    // Get ith char
    char& operator[](isize i)
    {
        assert(i >= 0);
        assert(i < len);
        return buf[i];
    }

    // Get slice
    Str operator[](isize beg, isize end)
    {
        assert(beg >= 0);
        assert(beg <= end);
        assert(end <= len);
        return {buf + beg, buf + end};
    }
};

#define pstr(x) (int)x.len, x.buf
