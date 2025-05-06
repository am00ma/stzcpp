#include "arena.h"
#include "log.h"

#include <cassert> // assert

static inline void oom(isize count, char* end, char* beg, isize pad, isize objsize)
{
    error("Out of memory:\n"                        //
          "  count > (end - beg - pad) / objsize\n" //
          "  %ld > (%p - %p - %ld) / %ld",
          count, end, beg, pad, objsize);
    exit(-1);
}

Arena::Arena(isize cap_)
{
    cap = cap_;
    beg = (char*)malloc(cap);
    end = beg ? beg + cap : 0;
}

char* Arena::Alloc(isize objsize, isize align, isize count, b32 flags)
{
    assert(count >= 0); // Can request 0?

    isize pad = -(uptr)beg & (align - 1); // Some way to approx mod(a,b)
    if (count > (end - beg - pad) / objsize)
    {
        if (flags & SOFTFAIL) return 0;
        oom(count, end, beg, pad, objsize);
        return 0; // TODO: OOM message with trace
    }

    isize total  = count * objsize;
    char* p      = beg + pad;
    beg         += pad + total;

    if (!(flags & NOZERO)) { p = (char*)memset(p, 0, total); }

    return p;
}

void Arena::Print(const char* label)
{
    printf("%s: used: %ld (beg: %p, end: %p, cap: %ld)\n", //
           label, cap - (end - beg),                       //
           (void*)beg, (void*)end, cap);
}
