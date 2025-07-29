#pragma once

#include "list.h"

typedef struct Str : List<char> {

    /* ---------------------------------------------------------------------------
     * Lifetime
     * ------------------------------------------------------------------------- */

    Str() = default;

    // From fields
    Str(char* buf_, isize len_)
    {
        buf = buf_;
        len = len_;
    }

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

    // From arena (zeroed, defaults, non-zeroed as needed)
    Str(isize len_, Arena* a, b32 flags = 0)
    {
        buf = a->Make<char>(len_, flags);
        len = len_;
    };

    /* ---------------------------------------------------------------------------
     * Operators - Having to copy to return Str instead of char
     * ------------------------------------------------------------------------- */

    // Equality
    bool operator==(Str s)
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
    char* operator[](isize i)
    {
        Assert(((i >= -1 * len) && (i < len))); // Bounds check
        if (i < 0) { i = len + i; }             // Negative
        return &buf[i];                         // Return reference
    }

    // [i - j]th items by reference, supporting negative indices
    Str operator[](isize i, isize j)
    {
        // Bounds check
        Assert(((i >= -1 * len) && (i < len)));
        Assert(((j > -1 * len) && (j <= len)));

        // Negative to positive
        if (i < 0) { i = len + i; }
        if (j < 0) { j = len + j; }

        // Check overlap given assured both positive (so also for [i, i] for example)
        if ((i > j) && (j >= 0)) { return Str(&buf[i], 0); }; // 0 >= j > i

        // Guaranteed only if j >= i (if i==j, len=cap=0, but address is of ith element)
        return Str(&buf[i], j - i);
    }

    // [i - j]th items by copy, supporting negative indices (same as Copy)
    Str operator[](isize i, isize j, Arena* a)
    {
        Str slice = (*this)[i, j];
        if (slice.len)
        {
            // If on top of arena, return directly
            if ((char*)buf == a->beg - len)
            {
                // debug("[M] Copy avoided");
                return *this;
            }

            Str dst = Str(len, a);

            // len > 0 guaranteed
            // debug("[M] Copied");
            memcpy(dst.buf, buf, len);

            return dst;
        }
        return slice;
    }

    /* ---------------------------------------------------------------------------
     * Methods
     * ------------------------------------------------------------------------- */

    Str Copy(Arena* a)
    {
        // If on top of arena, return directly
        if ((char*)buf == a->beg - len)
        {
            // debug("[M] Copy avoided");
            return *this;
        }

        Str dst = Str(len, a);
        if (len)
        {
            // debug("[M] Copied");
            memcpy(dst.buf, buf, len);
        }
        return dst;
    }

} Str;
