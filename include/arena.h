#pragma once

#include "log.h"   // Fatal, debug
#include "range.h" // RANGE
#include "types.h" // isize, b32

#include <cstdio>  // printf
#include <cstdlib> // malloc, free
#include <new>     // Needed for proper behaviour of `new` in `Make` !!

/* ---------------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------------- */

// Arena on stack, exact behaviour as on heap
#define BufArena(a, buf, cap)                                                                                          \
    char  buf[cap] = {};                                                                                               \
    Arena a        = Arena(buf, cap);

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
     * Methods (Should we inline these things?)
     * ------------------------------------------------------------------------- */

    isize Used() { return cap - (end - beg); }

    // How many objects can be allocated? TODO: Check floor, ceil
    template <typename T, typename... A> isize Available()
    {
        isize pad = -(uptr)beg & (alignof(T) - 1);
        return (end - beg - pad) / sizeof(T);
    }

    // Wrapper with support for defaults and typing
    template <typename T, typename... A> T* Make(isize count = 1, b32 flags = 0, A... args)
    {
        // If count < 0, things are really messed up
        // Though, can request size 0, NOTE: will just get top of arena?
        Assert(count >= 0);

        // Compute leftover after accounting for alignment
        isize align   = alignof(T);
        isize objsize = sizeof(T);
        isize pad     = -(uptr)beg & (align - 1); // Some way to approx mod(a,b)

        // Check if we have enough space to allocate
        if (count > (end - beg - pad) / objsize)
        {
            // SOFTFAIL support
            if (flags & SOFTFAIL) return 0;

            // Else drop to debugger
            Fatal(-1, "Alloc failed: count: %ld < req: %ld; used: %ld / cap: %ld", //
                  count, (end - beg - pad / objsize), cap - (end - beg), cap);
        }

        // Advance the arena
        isize total  = count * objsize;
        char* p      = beg + pad;
        beg         += pad + total;
        // debug("[A] Used: %ld / Cap: %ld", cap - (end - beg), cap);

        // By default, zero initialized
        if (!(flags & NOZERO))
        {
            // debug("[A] Memset: %ld", total);
            p = (char*)memset(p, 0, total);
        }

        // Convert to proper type
        T* r = (T*)p;

        // Allows for zero / default init (Needs to be specially requested through flags)
        // NOTE: needs `#include <new>`: https://en.cppreference.com/w/cpp/language/new#Placement_new
        if (flags & DEFAULTS)
        {
            RANGE(i, count) { new ((void*)&r[i]) T(args...); }
        }

        // Return with proper type info
        return r;
    }

    /* ---------------------------------------------------------------------------
     * Debugging
     * ------------------------------------------------------------------------- */

    void Print(const char* label)
    {
        debug("[A] %s: used: %ld / cap: %ld (beg: %p, end: %p)\n", //
              label, cap - (end - beg), cap, (void*)beg, (void*)end);
    }

} Arena;
