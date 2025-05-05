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

class Arena {
  private:
    char* beg = 0;
    char* end = 0;
    isize cap = 0;

  public:
    Arena(isize cap);
    ~Arena();

    char* Alloc(isize objsize, isize align, isize count, b32 flags);

    template <typename T> T* Make(isize count = 1, b32 flags = 0)
    {
        T* r = (T*)Alloc(sizeof(T), alignof(T), count, flags);

        // BUG:: Does not work,
        //
        //   include/arena.h:34:35: error: no matching function for call to ‘operator new(sizetype, void*)’
        //      34 |             RANGE(i, count) { (T*)new ((void*)&r[i]) T; }
        //         |                                   ^~~~~~~~~~~~~~~~~~~~
        //   <built-in>: note: candidate: ‘void* operator new(long unsigned int)’
        //   <built-in>: note:   candidate expects 1 argument, 2 provided
        //   <built-in>: note: candidate: ‘void* operator new(long unsigned int, std::align_val_t)’
        //   <built-in>: note:   no known conversion for argument 2 from ‘void*’ to ‘std::align_val_t’
        //
        // Allows for zero / default init
        if (flags & DEFAULTS)
        {
            RANGE(i, count) { new ((void*)&r[i]) T{}; } // https://en.cppreference.com/w/cpp/language/new#Placement_new
        }

        return r;
    }

    void Print(const char* label);
};
