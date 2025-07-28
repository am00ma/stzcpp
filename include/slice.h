#pragma once

#include "arena.h"
#include "log.h"

/* ---------------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------
 * Slice
 * ------------------------------------------------------------------------- */

template <typename T> struct Slice {

    // Hack for holding literals as well
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

    Slice(Arena* a, isize cap_)
    {
        len = 0;
        cap = cap_;
        buf = a->Make<T>(cap);
    };

    Slice(T* buf_, isize len_, isize cap_)
    {
        buf = buf_;
        len = len_;
        cap = cap_;
    };

    Slice(T* buf_, isize len_)
    {
        buf = buf_;
        len = len_;
        cap = len_;
    };

    template <isize N> constexpr Slice(const T (&s)[N])
    {
        buf = s;
        len = N;
        cap = N;
    };

    // Shrinks arena to len, releasing rest of cap
    // NOTE: Use wisely
    Slice<T> Final(Arena* a)
    {
        // TODO: Consider padding, so currently only for `char`
        Assert(sizeof(T) == 1);

        a->beg -= (cap - len) * sizeof(T);
        cap     = len;
        return *this;
    }

    /* ---------------------------------------------------------------------------
     * Operators
     * ------------------------------------------------------------------------- */

    // Identical to Append
    Slice<T>* operator+(T val)
    {
        if (len + 1 <= cap)
        {
            buf[len] = val;
            len++;
        }
        else { error("Overflow: len + 1 (%ld) <= cap (%ld)\nDropping item\n", len + 1, cap); }
        return this;
    }

    // Get ith item by reference, supporting negative indices
    T* operator[](isize i)
    {
        Assert(((i >= -1 * len) && (i < len))); // Bounds check
        if (i < 0) { i = len + i; }             // Negative
        return &buf[i];                         // Return reference
    };

    // Get (i - j)th item by reference, supporting negative indices
    Slice<T> operator[](isize i, isize j)
    {
        // Bounds check
        Assert(((i >= -1 * len) && (i < len)));
        Assert(((j >= -1 * len) && (j < len)));

        // Negative to positive
        if (i < 0) { i = len + i; }
        if (j < 0) { j = len + j; }

        // Check overlap given assured both positive (so also for [i, i] for example)
        if ((i > j) && (j >= 0)) { return Slice<T>(); }; // 0 >= j > i

        // Guaranteed only if j >= i (if i==j, len=cap=0, but address is of ith element)
        return Slice<T>(&buf[i], j - i, j - i);
    }

    // Sugar to copy subslice
    Slice<T> operator[](isize i, isize j, Arena* a)
    {
        Slice<T> slice = this[i, j];
        if (slice.len) { slice = slice.Copy(a); }
        return slice;
    }

    // Eqality
    bool operator==(Slice<T> s)
    {
        if (len != s.len) { return false; }
        if ((len == 0) && (buf == s.buf)) { return true; } // Takes care of null vs empty case
        RANGE(i, len)
        {
            if (buf[i] != *s[i]) return false; // Comparing by copy?
        }
        return true;
    }

    /* ---------------------------------------------------------------------------
     * Methods
     * ------------------------------------------------------------------------- */

    // Copy to arena
    Slice<T> Copy(Arena* a)
    {
        // If on top of arena, return directly TODO: Figure out alignment
        if (buf == a->beg - (len - sizeof(T))) { return *this; }

        Slice<T> dst = Slice<T>(a, len, len);
        if (len)
        {
            // debug("[M] Copied");
            memcpy(dst.buf, buf, len * sizeof(T));
        }
        return dst;
    }

    // Identical to operator '+', for ppl who dislike operators
    Slice<T>* Append(T val)
    {
        if (len + 1 <= cap)
        {
            buf[len] = val;
            len++;
        }
        else { error("Overflow: len + 1 (%ld) <= cap (%ld)\nDropping item\n", len + 1, cap); }
        return this;
    }

    /* ---------------------------------------------------------------------------
     * Debugging
     * ------------------------------------------------------------------------- */
};
