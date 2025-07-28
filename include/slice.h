#pragma once

#include "list.h"

template <typename T> struct Slice : List<T> {

    /* ---------------------------------------------------------------------------
     * Fields
     * ------------------------------------------------------------------------- */

    isize cap = 0;

    /* ---------------------------------------------------------------------------
     * Lifetime
     * ------------------------------------------------------------------------- */

    Slice() = default;

    // From fields
    Slice(T* buf_, isize len_, isize cap_)
    {
        List<T>::buf = buf_;
        List<T>::len = len_;
        cap          = cap_;
    }

    // From List (only len, full capacity)
    Slice(List<T> l)
    {
        List<T>::buf = l.buf;
        List<T>::len = l.len;
        cap          = l.len;
    }

    // From List (only len, full capacity)
    Slice(T* buf_, isize len_)
    {
        List<T>::buf = buf_;
        List<T>::len = len_;
        cap          = len_;
    }

    // From arena (zeroed, defaults, non-zeroed as needed)
    // With cap, but len = 0
    Slice<T>(isize cap_, Arena* a, b32 flags = 0)
    {
        List<T>::buf = a->Make<T>(cap_, flags);
        List<T>::len = 0;
        cap          = cap_;
    };

    Slice<T> Final(Arena* a); // Use with caution

    /* ---------------------------------------------------------------------------
     * Operators
     * ------------------------------------------------------------------------- */

    List<T> operator+(T val);       // Append
    List<T> operator+(List<T> val); // Extend

    /* ---------------------------------------------------------------------------
     * Methods
     * ------------------------------------------------------------------------- */

    List<T> Append(T val);       // Append
    List<T> Extend(List<T> val); // Extend

    /* ---------------------------------------------------------------------------
     * Debugging
     * ------------------------------------------------------------------------- */

    void Print() { debug("[L] len: %ld, cap: %ld, buf: %p", List<T>::len, cap, List<T>::buf); }
};
