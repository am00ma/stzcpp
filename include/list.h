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
    List<T>(isize len_, Arena* a, b32 flags = 0)
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
        if ((len == 0) && (buf == s.buf)) { return true; } // Takes care of null vs empty case
        RANGE(i, len)
        {
            if (buf[i] != *s[i]) return false; // Comparing by copy?
        }
        return true;
    }

    // By reference (supports negative indices)
    T* operator[](isize i)
    {
        Assert(((i >= -1 * len) && (i < len))); // Bounds check
        if (i < 0) { i = len + i; }             // Negative
        return &buf[i];                         // Return reference
    }

    // [i - j]th items by reference, supporting negative indices
    List<T> operator[](isize i, isize j)
    {
        // Bounds check
        Assert(((i >= -1 * len) && (i < len)));
        Assert(((j > -1 * len) && (j <= len)));

        // Negative to positive
        if (i < 0) { i = len + i; }
        if (j < 0) { j = len + j; }

        // Check overlap given assured both positive (so also for [i, i] for example)
        if ((i > j) && (j >= 0)) { return List<T>(&buf[i], 0); }; // 0 >= j > i

        // Guaranteed only if j >= i (if i==j, len=cap=0, but address is of ith element)
        return List<T>(&buf[i], j - i);
    }

    // [i - j]th items by copy, supporting negative indices
    List<T> operator[](isize i, isize j, Arena* a)
    {
        List<T> slice = (*this)[i, j];
        if (slice.len) { slice = slice.Copy(a); }
        return slice;
    }

    /* ---------------------------------------------------------------------------
     * Methods
     * ------------------------------------------------------------------------- */

    List<T> Copy(Arena* a)
    {
        // If on top of arena, return directly
        // TODO: Why doesn't padding and alignment mess up things here?
        if ((char*)buf == a->beg - (len * sizeof(T)))
        {
            // debug("[M] Copy avoided");
            return *this;
        }

        List<T> dst = List<T>(len, a);
        if (len)
        {
            // debug("[M] Copied");
            memcpy(dst.buf, buf, len * sizeof(T));
        }
        return dst;
    }

    /* ---------------------------------------------------------------------------
     * Debugging
     * ------------------------------------------------------------------------- */

    void Print() { debug("[L] len: %ld, buf: %p", len, buf); }
};
