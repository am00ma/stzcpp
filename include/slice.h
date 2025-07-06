#pragma once

#include "arena.h"
#include "range.h"
#include <cassert>

template <typename T> struct Slice {
    T*    data;
    isize len;
    isize cap;

    Slice() = default;
    Slice(T* data_, isize len_, isize cap_)
    {
        data = data_;
        len  = len_;
        cap  = cap_;
    };
    Slice(Arena* a, isize cap_)
    {
        len  = 0;
        cap  = cap_;
        data = a->Make<T>(cap);
    };

    void Append(T val)
    {
        assert(len + 1 <= cap);
        data[len] = val;
        len++;
    }

    // Get ith item
    T* operator[](isize i)
    {
        assert(i >= 0);
        assert(i < len);
        return &data[i];
    }

    // Get (i - j)th item
    Slice<T> operator[](isize i, isize j)
    {
        assert(i >= 0);
        assert(j >= i);
        assert(j < len);
        return Slice<T>(&data[i], j - i, j - i);
    }

    // str_equal
    b32 operator==(Slice<T> s)
    {
        if (len != s.len) { return false; }
        RANGE(i, len)
        {
            if (data[i] != s[i]) return false;
        }
        return true;
    }
};
