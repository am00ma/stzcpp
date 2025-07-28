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

    // Shrinks arena
    // NOTE: provided no new objects after declaration of Buf
    List<T> Final(Arena* a)
    {
        a->beg -= cap - (List<T>::len * sizeof(T));
        cap     = List<T>::len; // So nothing can be added later
        return List<T>(List<T>::buf, List<T>::len);
    };

    /* ---------------------------------------------------------------------------
     * Operators
     * ------------------------------------------------------------------------- */

    // Append an item and increment len, exactly same as Append
    List<T> operator+(T val)
    {
        if (List<T>::len + 1 <= cap)
        {
            List<T>::buf[List<T>::len] = val;
            List<T>::len++;
        }
        else { error("Overflow: len + 1 (%ld) <= cap (%ld)\nDropping item\n", List<T>::len + 1, cap); }
        return *this;
    }

    // Extend slice by copying items (unrolled operator+)
    List<T> operator+(List<T> val)
    {
        RANGE(i, val.len)
        {
            if (List<T>::len + 1 <= cap)
            {
                List<T>::buf[List<T>::len] = *val[i];
                List<T>::len++;
            }
            else { error("Overflow: len + 1 (%ld) <= cap (%ld)\nDropping item\n", List<T>::len + 1, cap); }
        }
        return List<T>(List<T>::buf, List<T>::len);
    }

    /* ---------------------------------------------------------------------------
     * Methods
     * ------------------------------------------------------------------------- */

    // Exactly same as operator+ to append item
    List<T> Append(T val)
    {
        if (List<T>::len + 1 <= cap)
        {
            List<T>::buf[List<T>::len] = val;
            List<T>::len++;
        }
        else { error("Overflow: len + 1 (%ld) <= cap (%ld)\nDropping item\n", List<T>::len + 1, cap); }
        return List<T>(List<T>::buf, List<T>::len);
    }

    // Exactly same as operator+ to extend slice, unrolled
    List<T> Extend(List<T> val)
    {
        RANGE(i, val.len)
        {
            if (List<T>::len + 1 <= cap)
            {
                List<T>::buf[List<T>::len] = *val[i];
                List<T>::len++;
            }
            else { error("Overflow: len + 1 (%ld) <= cap (%ld)\nDropping item\n", List<T>::len + 1, cap); }
        }
        return List<T>(List<T>::buf, List<T>::len);
    }

    /* ---------------------------------------------------------------------------
     * Debugging
     * ------------------------------------------------------------------------- */

    void Print() { debug("[L] len: %ld, cap: %ld, buf: %p", List<T>::len, cap, List<T>::buf); }
};
