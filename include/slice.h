#pragma once

#include "list.h"

template <typename T> struct Slice {

    // TODO: Why are these not inherited from List<T>?
    // Hack for holding, init from const literals as well
    union {
        T*       buf = 0;
        const T* cbuf;
    };
    isize len = 0;

    isize cap = 0;

    /* ---------------------------------------------------------------------------
     * Lifetime
     * ------------------------------------------------------------------------- */

    Slice() = default;

    // From fields
    Slice(T* buf_, isize len_, isize cap_)
    {
        buf = buf_;
        len = len_;
        cap = cap_;
    }

    // From List (only len, full capacity)
    Slice(List<T> l)
    {
        buf = l.buf;
        len = l.len;
        cap = l.len;
    }

    // From List (only len, full capacity)
    Slice(T* buf_, isize len_)
    {
        buf = buf_;
        len = len_;
        cap = len_;
    }

    // From literals
    template <isize N> constexpr Slice(T (&s)[N])
    {
        buf = s;
        len = N;
        cap = N;
    }

    // From const literals
    template <isize N> constexpr Slice(const T (&s)[N])
    {
        cbuf = s;
        len  = N;
        cap  = N;
    }

    // From arena (zeroed, defaults, non-zeroed as needed)
    // With cap, but len = 0
    Slice<T>(isize cap_, Arena* a, b32 flags = 0)
    {
        buf = a->Make<T>(cap_, flags);
        len = 0;
        cap = cap_;
    };

    Slice<T> Final(Arena* a); // Use with caution

    /* ---------------------------------------------------------------------------
     * Operators
     * ------------------------------------------------------------------------- */

    // Equality (copied from list)
    bool operator==(List<T> s)
    {
        if (len != s.len) { return false; }
        if ((len == 0) && (buf == s.buf)) { return true; }
        RANGE(i, len)
        {
            if (buf[i] != *s[i]) return false;
        }
        return true;
    }

    // Equality (copied from list)
    bool operator==(Slice<T> s)
    {
        if (len != s.len) { return false; }
        if ((len == 0) && (buf == s.buf)) { return true; }
        RANGE(i, len)
        {
            if (buf[i] != *s[i]) { return false; }
        }
        return true;
    }

    // By reference, supports negative indices (copied from List)
    T* operator[](isize i)
    {
        Assert(((i >= -1 * len) && (i < len))); // Bounds check
        if (i < 0) { i = len + i; }             // Negative
        return &buf[i];                         // Return reference
    }

    // [i - j]th by reference, supporting negative indices (copied from List)
    List<T> operator[](isize i, isize j)
    {
        Assert(((i >= -1 * len) && (i < len)));
        Assert(((j > -1 * len) && (j <= len)));
        if (i < 0) { i = len + i; }
        if (j < 0) { j = len + j; }
        if ((i > j) && (j >= 0)) { return List<T>(&buf[i], 0); }; // 0 >= j > i
        return List<T>(&buf[i], j - i);
    }

    // [i - j]th items by copy, supporting negative indices (copied from List)
    List<T> operator[](isize i, isize j, Arena* a)
    {
        List<T> slice = (*this)[i, j];
        if (slice.len) { slice = slice.Copy(a); }
        return slice;
    }

    List<T> operator+(T val); // Append

    List<T> operator+(List<T> val); // Extend

    /* ---------------------------------------------------------------------------
     * Methods
     * ------------------------------------------------------------------------- */

    // copied from List
    List<T> Copy(Arena* a)
    {
        if ((char*)buf == a->beg - (len * sizeof(T)))
        {
            // debug("[M] Copy avoided");
            return List<T>{buf, len};
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

    void Print() { debug("[L] len: %ld, cap: %ld, buf: %p", len, cap, buf); }
};
