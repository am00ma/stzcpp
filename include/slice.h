#pragma once

#include "list.h"

template <typename T> struct Slice : List<T> {

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

    List<T> operator+(T val);       // Append
    List<T> operator+(List<T> val); // Extend
    List<T> Final(Arena* a);        // Use with caution
};
