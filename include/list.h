#pragma once

#include "arena.h"

template <typename T> struct List {

    /* ---------------------------------------------------------------------------
     * Fields
     * ------------------------------------------------------------------------- */

    // Hack for holding, init from const literals as well
    union {
        T*       buf = 0;
        const T* cbuf;
    };
    isize len = 0;

    /* ---------------------------------------------------------------------------
     * Lifetime
     * ------------------------------------------------------------------------- */

    List() = default;

    // From fields
    List(T* buf_, isize len_)
    {
        buf = buf_;
        len = len_;
    }

    // From literals
    template <isize N> constexpr List(T (&s)[N])
    {
        buf = s;
        len = N;
    }

    // From const literals
    template <isize N> constexpr List(const T (&s)[N])
    {
        cbuf = s;
        len  = N;
    }

    // From arena (zeroed, defaults, non-zeroed as needed)
    List<T>(Arena* a, isize len_, b32 flags = 0)
    {
        buf = a->Make<T>(len_, flags);
        len = len_;
    };

    /* ---------------------------------------------------------------------------
     * Operators
     * ------------------------------------------------------------------------- */

    // Equality
    bool operator==(List<T> s)
    {
        if (len != s.len) { return false; }
        if ((len == 0) && (s.len == 0)) { return true; } // Empty strings are equal
        RANGE(i, len) if (buf[i] != *s[i]) return false;
        return true;
    }

    // By reference (supports negative indices)
    T* operator[](isize i)
    {
        if (!((i >= -1 * len) && (i < len))) { return 0; };
        if (i < 0) { i = len + i; }
        return &buf[i];
    }

    // [i - j]th items by reference, supporting negative indices
    List<T> operator[](isize i, isize j)
    {
        if ((i >= len) || (j <= -1 * len)) { return List(); }
        if ((i <= -1 * len) && (j >= len)) { return *this; }
        if (j >= len) { j = len; }
        if (i <= -1 * len) { i = -1 * len; }
        if (i < 0) { i = len + i; }
        if (j < 0) { j = len + j; }
        if (i >= j) { return List(&buf[i], 0); };
        return List(&buf[i], j - i);
    }

    /* ---------------------------------------------------------------------------
     * Methods
     * ------------------------------------------------------------------------- */

    // Copy to arena
    List<T> Copy(Arena* a)
    {
        if ((char*)buf == a->beg - (len * sizeof(T))) { return *this; }
        List<T> dst = List<T>(a, len);
        if (len) { memcpy(dst.buf, buf, len * sizeof(T)); }
        return dst;
    }

    /* ---------------------------------------------------------------------------
     * Debugging
     * ------------------------------------------------------------------------- */

    void Print() { debug("[L] len: %ld, buf: %p", len, buf); }
};
