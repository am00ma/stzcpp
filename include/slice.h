#pragma once
/*
 * Slice:
 *
 *   Fields:
 *
 *      T*    buf = 0;
 *      isize len = 0;
 *      isize cap = 0;
 *
 *   Lifetime:
 *
 *      Slice();
 *
 *      Slice(T* buf, isize len_, isize cap_);               // From buf
 *      Slice(T* buf, isize len_);                           // len = cap
 *      Slice(Arena* a, isize cap_);                         // On arena
 *      template <isize N> constexpr Slice(T (&s)[N]);       // From literals
 *
 *      Slice<T> Final(Arena* a); // Use with caution
 *
 *   Operators:
 *
 *      Slice<T>* operator+ (T val);                      // Append
 *      T*        operator[](isize i);                    // By reference
 *      Slice<T>  operator[](isize i, isize j);           // By reference
 *      Slice<T>  operator[](isize i, isize j, Arena* a); // By copy
 *      bool      operator==(Slice<T> s);                 // Equality
 *
 *   Methods:
 *
 *      Slice<T>* Append(T val);
 *
 *   Debugging:
 *
 *      void Print();
 *
 * Notes:
 *
 *  1. `Slice(T* buf, isize len_);` is convinience to set len equal to cap on init
 *  2. `Slice<T> Final(Arena* a);` is tricky but very useful
 *  3. `cap` is needed only for debugging and `Free`
 *
 * */

#include "arena.h"

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

    // TODO: Not working
    template <isize N> constexpr Slice(const T (&s)[N])
    {
        buf = (T*)s;
        len = N;
        cap = N;
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

    // Get (i - j)th item as copy, supporting negative indices
    Slice<T> operator[](isize i, isize j, Arena* a)
    {
        // Bounds check
        Assert(((i >= -1 * len) && (i < len)));
        Assert(((j > -1 * len) && (j <= len)));

        // Negative to positive
        if (i < 0) { i = len + i; }
        if (j < 0) { j = len + j; }

        // Check overlap given assured both positive (so also for [i, i] for example)
        if ((i >= j) && (j >= 0)) { return Slice<T>(); }; // 0 >= j > i

        // NOTE: here behavior seems to differ from get by ref for i == j

        // Diff from operator[] by ref -> we are making Slice
        isize len   = j - i;
        auto  slice = Slice<T>(a->Make<T>(len), len, len);
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
};
