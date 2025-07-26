#pragma once

#include "arena.h"
#include "range.h"

template <typename T> struct Slice {

    T*    buf = 0;
    isize len = 0;
    isize cap = 0;

    Slice() = default;

    Slice(Arena* a, isize cap_)
    {
        len = 0;
        cap = cap_;
        buf = a->Make<T>(cap);
    };

    Slice(T* data_, isize len_, isize cap_)
    {
        buf = data_;
        len = len_;
        cap = cap_;
    };

    Slice(T* data_, isize len_)
    {
        buf = data_;
        len = len_;
        cap = len_;
    };

    // Shrinks arena to len, releasing rest of cap
    // NOTE: provided no new objects after declaration of Slice
    Slice<T> Final(Arena* a)
    {
        a->beg -= (cap - len) * sizeof(T);
        cap     = len;
        return *this;
    }

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

    // Get ith item by reference
    T* operator[](isize i)
    {
        Assert(i >= 0);
        Assert(i < len);
        return &buf[i];
    }

    // Get (i - j)th item by reference
    Slice<T> operator[](isize i, isize j)
    {
        Assert(i >= 0);
        Assert(j >= i);
        Assert(j < len);
        return Slice<T>(&buf[i], j - i, j - i);
    }

    // Get (i - j)th item as copy
    Slice<T> operator[](isize i, isize j, Arena* a)
    {
        Assert(i >= 0);
        Assert(j >= i);
        Assert(j < len);
        auto slice = Slice<T>(a->Make<T>(j - i), j - i, j - i);
        if (slice.len) memcpy(slice.buf, &buf[i], slice.len * sizeof(T));
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

    // Identical to operator '+'
    void Append(T val)
    {
        if (len + 1 <= cap)
        {
            buf[len] = val;
            len++;
        }
        else { error("Overflow: len + 1 (%ld) <= cap (%ld)\nDropping item\n", len + 1, cap); }
    }
};
