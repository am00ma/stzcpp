#pragma once

#include "range.h" // RANGE
#include "types.h" // isize, b32

#include <new> // Needed for proper behaviour of new !!

#include <cstdio>  // printf
#include <cstdlib> // malloc

/* ---------------------------------------------------------------------------
 * Arena
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

    // Default constructor
    Arena() = default;

    // Lifetime
    Arena(isize cap_);
    Arena(isize cap_, Arena* src);
    Arena(char* buf, isize cap_);

    // No free by destructor
    // as only applicable if it wasnt allocated with constructors
    void Free() { free(end - cap); };

    // Meat
    char* Alloc(isize objsize, isize align, isize count, b32 flags);

    // Syntactic sugar
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

    inline isize Used() { return cap - (end - beg); }

    char* OrigBeg() { return (end - cap); };

    // Debug
    void Print(const char* label);

} Arena;

#define BufArena(a, buf, cap)                                                                                          \
    char  buf[cap] = {};                                                                                               \
    Arena a        = Arena(buf, cap);
