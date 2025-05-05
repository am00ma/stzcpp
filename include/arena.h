#pragma once

#include "range.h" // RANGE
#include "types.h" // isize, b32

#include <new> // Needed for proper behaviour of new !!

/* ---------------------------------------------------------------------------
 * Arena (64 MB)
 * ------------------------------------------------------------------------- */
typedef enum {
    NOZERO   = 0x1,
    SOFTFAIL = 0x2,
    DEFAULTS = 0x4,
} ArenaFlags;

typedef struct Arena {
    char* beg = 0;
    char* end = 0;
    isize cap = 0;

    Arena(isize cap);
    ~Arena();

    char* Alloc(isize objsize, isize align, isize count, b32 flags);

    template <typename T, typename... A> T* Make(isize count = 1, b32 flags = 0, A... args)
    {
        T* r = (T*)Alloc(sizeof(T), alignof(T), count, flags);

        // Allows for zero / default init NOTE: needs `#include <new>`
        // https://en.cppreference.com/w/cpp/language/new#Placement_new
        if (flags & DEFAULTS)
        {
            RANGE(i, count) { new ((void*)&r[i]) T(args...); }
        }

        return r;
    }

    void Print(const char* label);
} Arena;
