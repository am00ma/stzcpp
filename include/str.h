#pragma once

#include "slice.h"

typedef struct Str : List<char> {

    Str() = default;

    // From literals
    template <isize N> constexpr Str(char (&s)[N])
    {
        buf = s;
        len = N - 1;
    }

    // From const literals
    template <isize N> constexpr Str(const char (&s)[N])
    {
        cbuf = s;
        len  = N - 1;
    }

    // From List
    Str(List<char> l)
    {
        buf = l.buf;
        len = l.len;
    }

} Str;

typedef struct Buf : Slice<char> {

    Buf() = default;

    // Conversion from List
    Buf(List<char> l)
    {
        buf = l.buf;
        len = l.len;
        cap = l.len;
    };

    // Conversion from Str
    Buf(Str l)
    {
        buf = l.buf;
        len = l.len;
        cap = l.len;
    };

    // Conversion from Slice
    Buf(Slice<char> b)
    {
        buf = b.buf;
        len = b.len;
        cap = b.cap;
    };

    // Canonical usage to init from static buf (not literals, i.e. no const)
    template <isize N> constexpr Buf(char (&s)[N])
    {
        List<char>::buf = s;
        List<char>::len = 0;
        cap             = N;
    }

} Buf;
