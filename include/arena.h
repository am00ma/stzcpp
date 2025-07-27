#pragma once
/*
 * Arena:
 *
 *   Fields:
 *
 *      char* beg = 0;
 *      char* end = 0;
 *      isize cap = 0;
 *
 *   Lifetime:
 *
 *      Arena();
 *
 *      Arena(isize cap_);
 *      Arena(isize cap_, Arena* src, b32 flags = 0);
 *      Arena(char* buf, isize cap_);
 *
 *      void Free();
 *
 *   Operators:
 *
 *   Methods:
 *
 *      char* Alloc(isize objsize, isize align, isize count, b32 flags);
 *      T*    Make<V>(isize count = 1, b32 flags = 0, A... args);
 *
 *   Debugging:
 *
 *      isize Used();
 *      isize OrigBeg();
 *      void  Print();
 *
 * Notes:
 *
 *  1. Free() always has signature `void Free();`
 *  2. Only method really used is `Make<V>`
 *  3. `cap` is needed only for debugging and `Free`
 *
 * */

#include "log.h"   // error
#include "range.h" // RANGE
#include "types.h" // isize, b32

#include <new> // Needed for proper behaviour of `new` in `Make` !!

#include <cstdio>  // printf
#include <cstdlib> // malloc, free

/* ---------------------------------------------------------------------------
 * Flags for allocation
 * ------------------------------------------------------------------------- */
typedef enum {

    NOZERO   = 0x1,
    SOFTFAIL = 0x2,
    DEFAULTS = 0x4,

} ArenaFlags;

/* ---------------------------------------------------------------------------
 * Arena
 * ------------------------------------------------------------------------- */
typedef struct Arena {

    char* beg = 0;
    char* end = 0;
    isize cap = 0;

    /* ---------------------------------------------------------------------------
     * Lifetime
     * ------------------------------------------------------------------------- */

    // Default constructor
    Arena() = default;

    // Use `malloc`, 0 capacity arena if malloc fails
    Arena(isize cap_)
    {
        cap = cap_;
        beg = (char*)malloc(cap);
        end = beg ? beg + cap : 0;
    }

    // Take from another arena
    Arena(isize cap_, Arena* src, b32 flags = 0)
    {
        cap = cap_;
        beg = src->Make<char>(cap);
        end = beg ? beg + cap : 0;
    }

    // Given buffer and capacity
    Arena(char* buf, isize cap_)
    {
        beg = buf;
        cap = beg ? cap_ : 0; // Zero-cap arena if invalid buf
        end = beg ? beg + cap : 0;
    }

    // No free by destructor
    //    ( as only applicable if it was allocated
    //      with malloc constructor: Arena(isizze cap_) )
    void Free()
    {
        if (end - cap) free(end - cap);
    }

    /* ---------------------------------------------------------------------------
     * Methods
     * ------------------------------------------------------------------------- */

    // Generic allocation function
    char* Alloc(isize objsize, isize align, isize count, b32 flags)
    {
        Assert(count >= 0); // Can request 0?

        isize pad = -(uptr)beg & (align - 1); // Some way to approx mod(a,b)
        if (count > (end - beg - pad) / objsize)
        {
            if (flags & SOFTFAIL) return 0;

            Fatal(-1, "Alloc failed: count: %ld < req: %ld; used: %ld / cap: %ld", count, (end - beg - pad / objsize),
                  cap - (end - beg), cap);
            return 0;
        }

        isize total  = count * objsize;
        char* p      = beg + pad;
        beg         += pad + total;

        if (!(flags & NOZERO)) { p = (char*)memset(p, 0, total); }

        return p;
    }

    // Wrapper with support for defaults and typing
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

    /* ---------------------------------------------------------------------------
     * Operators (Cannot really take T as argument unfortunately)
     * ------------------------------------------------------------------------- */

    /* ---------------------------------------------------------------------------
     * Debugging
     * ------------------------------------------------------------------------- */

    inline isize Used() { return cap - (end - beg); }
    inline char* OrigBeg() { return (end - cap); };

    void Print(const char* label)
    {
        printf("%s: used: %ld / cap: %ld (beg: %p, end: %p)\n", //
               label, cap - (end - beg), cap, (void*)beg, (void*)end);
    }

} Arena;

/* ---------------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------------- */

#define BufArena(a, buf, cap)                                                                                          \
    char  buf[cap] = {};                                                                                               \
    Arena a        = Arena(buf, cap);
