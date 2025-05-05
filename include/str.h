#pragma once

#include "arena.h" // Arena
#include "types.h" // isize
#include <cassert> // assert

/* ---------------------------------------------------------------------------
 * Counted string
 * ------------------------------------------------------------------------- */
typedef struct Str Str;

typedef struct Strs {
    Str*  data;
    isize len;

    // Default constructor
    Strs() = default;

    // Lifetime
    Strs(Arena* a, isize len_);

} Strs;

typedef struct Str {
    // Hack for holding string literals as well
    union {
        char*       buf = 0;
        const char* cbuf;
    };
    isize len = 0;

    // Default constructor
    Str() = default;

    // From string literals
    template <isize N> constexpr Str(const char (&s)[N])
    {
        cbuf = s;
        len  = N - 1;
    }

    // From fields
    Str(char* buf_, isize len_);

    // From Arena
    Str(Arena* a, isize len_);

    // From formatted string
    Str(Arena* a, isize len, char const* fmt, ...);

    // From spans
    Str(char* beg, char* end);

    // Get ith char
    char& operator[](isize i);

    // Get slice
    Str operator[](isize beg, isize end);

    // str_equal
    b32 operator==(Str s);

    // Lifetime
    Str Copy(Arena* a, bool null_terminate = false);

    // Null terminated string
    char* Cstr(Arena* a, Str s);

    // Split (defaults to splitting lines)
    Strs Split(Arena* a, Str delimiter = "\n", bool ignore_empty = true, bool substitute_null = false);

} Str;

#define pstr(x) (int)x.len, x.buf
