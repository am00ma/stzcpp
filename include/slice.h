#pragma once

#include "list.h"
#include "log.h"

// Similar to std::vector, but not dynamic
template <typename T> struct Slice : List<T> {

    /* ---------------------------------------------------------------------------
     * Fields
     * ------------------------------------------------------------------------- */

    isize cap = 0;

    /* ---------------------------------------------------------------------------
     * Lifetime
     * ------------------------------------------------------------------------- */

    Slice() = default;

    // From fields, in order
    Slice(T* buf_, isize len_, isize cap_)
    {
        List<T>::buf = buf_;
        List<T>::len = len_;
        cap          = cap_;
    }

    // From buf (only cap, so assume empty)
    Slice(T* buf_, isize cap_)
    {
        List<T>::buf = buf_;
        List<T>::len = 0;
        cap          = cap_;
    }

    // From List (cap = len)
    Slice(List<T> l)
    {
        List<T>::buf = l.buf;
        List<T>::len = l.len;
        cap          = l.len;
    }

    // From arena (cap = cap_, len = 0; zeroed, defaults, non-zeroed)
    Slice<T>(Arena* a, isize cap_, b32 flags = 0)
    {
        List<T>::buf = a->Make<T>(cap_, flags);
        List<T>::len = 0;
        cap          = cap_;
    };

    // Shrinks arena
    // NOTE: provided no new objects after declaration of Buf
    List<T> Shrink(Arena* a)
    {
        // Can check above condition like in Copy!
        a->beg -= cap - (List<T>::len * sizeof(T));
        cap     = List<T>::len; // So nothing can be added later
        return List<T>(List<T>::buf, List<T>::len);
    };

    /* ---------------------------------------------------------------------------
     * Operators
     * ------------------------------------------------------------------------- */

    // Append item, increment len
    Slice<T>& operator+=(T val)
    {
        Assert(List<T>::len < cap);
        List<T>::buf[List<T>::len] = val;
        List<T>::len++;
        return *this;
    }

    // Extend slice, increment len , TODO: prob can do this with memcpy
    Slice<T>& operator+=(List<T> val)
    {
        Assert(List<T>::len + val.len <= cap);
        if (val.len) { memcpy(val.buf, List<T>::buf, val.len * sizeof(T)); }
        List<T>::len += val.len;
        return *this;
    }

    /* ---------------------------------------------------------------------------
     * Methods
     * ------------------------------------------------------------------------- */

    // enum { SLICE_INITIAL_CAP = 4 };

    // void *push_(Arena *a, void *data, ptrdiff_t *pcap, ptrdiff_t size)
    // {
    //     ptrdiff_t cap   = *pcap;
    //     ptrdiff_t align = _Alignof(void *);
    //
    //     if (!data || a->beg != (char *)data + cap*size) {
    //         void *copy = alloc(a, cap, size, align);
    //         if (data) memcpy(copy, data, cap*size);
    //         data = copy;
    //     }
    //
    //     ptrdiff_t extend = cap ? cap : SLICE_INITIAL_CAP;
    //     alloc(a, extend, size, 1);  // already aligned
    //     *pcap = cap + extend;
    //     return data;
    // }

    /* ---------------------------------------------------------------------------
     * Debugging
     * ------------------------------------------------------------------------- */

    void Print() { debug("[L] len: %ld, cap: %ld, buf: %p", List<T>::len, cap, List<T>::buf); }
};
