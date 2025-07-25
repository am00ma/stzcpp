#pragma once

#include "arena.h" // Arena
#include "types.h" // isize

/* ---------------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------------- */

// For printing with `%.*s`
#define pstr(x)  (int)x.len, x.buf
#define ppstr(x) (int)x->len, x->buf

// List of strings, for ops like split
typedef struct Str  Str;
typedef struct Strs Strs;

/* ---------------------------------------------------------------------------
 * Counted string
 * ------------------------------------------------------------------------- */
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

    // From const
    Str(const char* buf_);

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
    bool operator==(Str s);

    // Null terminated string
    char* Cstr(Arena* a);

    // Copy to arena
    Str Copy(Arena* a, bool null_terminate = false);

    // Split (defaults to splitting lines)
    Strs Split(Arena* a, Str delimiter = "\n", bool ignore_empty = true, bool substitute_null = false);

    // FNV hash
    u64 Hash64();

    // For debugging
    void Print(const char* label);

} Str;

typedef struct Strs {

    Str*  data;
    isize len;

    Strs() = default;
    Strs(Arena* a, isize len_);
    Strs(Str* data_, isize len_);

    Str* operator[](isize i)
    {
        if (i < 0) return 0;
        return &data[i];
    };

} Strs;
