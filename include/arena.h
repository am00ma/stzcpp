#pragma once

#include "types.h" // isize, b32

/* ---------------------------------------------------------------------------
 * Arena (64 MB)
 * ------------------------------------------------------------------------- */
typedef enum {
    NOZERO   = 0x1,
    SOFTFAIL = 0x2,
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

    template <typename T> T* New(isize count = 1, b32 flags = 0)
    {
        return (T*)Alloc(sizeof(T), alignof(T), count, flags);
    }

    void Print(const char* label);
};
