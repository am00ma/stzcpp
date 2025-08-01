#include "doctest.h"
#include "range.h"
#include <cassert>
#include <cstdio>

#include <new> // Needed for proper behaviour of `new` in `Make` !!

constexpr u64 mask = 0x0000ffff;
u64           orig = 0;

#define newarena(a, buf, n)                                                                                            \
    char  buf[n] = {};                                                                                                 \
    Arena a      = Arena(buf, n);                                                                                      \
    orig         = (u64)a.beg & mask;

#define print_arena_head()                                                                                             \
    printf("| %6s | %6s | %5s | %5s  | %5s | %6s | %6s || %6s | %6s | %6s | %6s |\n", ".", "name", "size", "align",    \
           "n", "n*size", "pad", "cap", "used", "beg", "end");                                                         \
    printf("| ------ | ------ | ----- | -----  | ----- | ----- | ------ || ------ | ------ | ------ | ------ |\n");

#define print_arena(l, a, r)                                                                                           \
    printf("| %6ld | %6s | %5ld | %5ld  | %5ld | %6ld | %6ld || %6ld | %6ld | %6ld | %6ld |\n", l, types[r.t].name,    \
           types[r.t].size, types[r.t].align, r.n, r.n* types[r.t].size, pad, cap, cap - (end - beg),                  \
           ((u64)beg & mask) - orig, ((u64)end & mask) - orig);

#define print_type_head()                                                                                              \
    printf("| %6s | %5s | %5s |\n", "name", "size", "align");                                                          \
    printf("| %6s | %5s | %5s |\n", "----", "----", "-----");
#define print_type(t) printf("| %6s | %5ld | %5ld |\n", t.name, t.size, t.align);

#define print_pad_head()                                                                                               \
    printf("| %5s | %5s | %5s |\n", "beg", "align", "pad");                                                            \
    printf("| %5s | %5s | %5s |\n", "---", "-----", "---");
#define print_pad() printf("| %5ld | %5ld | %5ld |\n", beg, align, pad);

typedef struct {
    const char* name;
    isize       size;
    isize       align;
} Type;

typedef enum {
    CHAR = 0,
    U16,
    U16x3,
    U32x3,
    A,
    B,
    C,
    D,
    E,
} Types;

Type types[] = {
    {"char", 1, 1}, {"u16", 2, 2}, {"u16[3]", 6, 2}, {"u32[3]", 12, 4}, {"a", 17, 4},
    {"b", 9, 4},    {"c", 21, 8},  {"d", 5, 1},      {"e", 3, 2},
};
isize n = sizeof(types) / sizeof(Type);

typedef struct {
    Types t;
    isize n;
} Req;

Req reqs[] = {
    {CHAR, 3}, {U16, 3},   {U16x3, 5}, {U32x3, 7}, {A, 11}, {B, 13}, {C, 11}, {D, 7}, {E, 5}, {CHAR, 0},
    {U16, 3},  {U16x3, 5}, {CHAR, 3},  {U32x3, 7}, {A, 11}, {B, 13}, {C, 11}, {D, 7}, {E, 5}, {CHAR, 0},
};
isize m = sizeof(reqs) / sizeof(Req);

typedef enum {

    NOZERO   = 0x1,
    SOFTFAIL = 0x2,
    DEFAULTS = 0x4,

} ArenaFlags;

typedef struct Arena {
    char* beg = 0;
    char* end = 0;
    isize cap = 0;

    Arena(char* buf, isize cap_)
    {
        beg = buf;
        cap = beg ? cap_ : 0;
        end = beg ? beg + cap : 0;
    }

    char* Make(isize i, Req r)
    {
        isize align = types[r.t].align;
        isize size  = types[r.t].size;

        // Compute sizes
        // align is always power of 2    ; e.g.  align = 0x01000 (8)
        // therefore (-1) always gives 1s; (align - 1) = 0x00111 (7)
        // And, always positive
        isize pad = -(uptr)beg & (types[r.t].align - 1);
        if (r.n > (end - beg - pad) / size) { assert("Alloc failed"); }

        // Advance arena
        isize total  = r.n * size;
        char* p      = beg + pad;
        beg         += pad + total;

        print_arena(i, this, reqs[i]);

        return p;
    }

    // How many objects can be allocated? TODO: Check floor, ceil
    template <typename T, typename... A> isize Available()
    {
        isize pad = -(uptr)beg & (alignof(T) - 1);
        return (end - beg - pad) / sizeof(T);
    }

    // Wrapper with support for defaults and typing
    template <typename T, typename... A> T* RealMake(isize count = 1, b32 flags = 0, A... args)
    {
        // Null behaviour
        if (count == 0) return 0;

        // If count < 0, things are really messed up
        Assert(count > 0);

        // Compute leftover after accounting for alignment
        isize align   = alignof(T);
        isize objsize = sizeof(T);
        isize pad     = -(uptr)beg & (align - 1); // Some way to approx mod(a,b)
        // debug("pad: %ld , align: %ld, size: %ld", pad, alignof(T), sizeof(T));

        // Check if we have enough space to allocate
        if (count > (end - beg - pad) / objsize)
        {
            // SOFTFAIL support
            if (flags & SOFTFAIL) return 0;

            // Else drop to debugger
            Assert(false);
        }

        // Advance the arena
        isize total  = count * objsize;
        char* p      = beg + pad;
        beg         += pad + total;
        // debug("[A] Used: %ld / Cap: %ld (pad: %ld)", cap - (end - beg), cap, pad);

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

} Arena;

int main()
{
    TEST_SUITE("Allocation");

    TEST_CASE("Types")
    {
        print_type_head();
        RANGE(i, n) { print_type(types[i]); }
    }

    newarena(a, buf, 1024);

    TEST_CASE("Make")
    {
        print_arena_head();
        RANGE(i, m) { a.Make(i, reqs[i]); }
    }

    TEST_CASE("Pad")
    {
        print_pad_head();
        isize aligns[] = {1, 2, 4, 8, 16};
        RANGE(beg, ((isize)a.beg & 0xFF), (((isize)a.beg & 0xFF) + 3))
        {
            RANGE(j, 5)
            {
                isize align = aligns[j];
                isize pad   = -(uptr)beg & (align - 1);
                print_pad();
            }
        }
    }

    TEST_CASE("RealMake")
    {
        TEqualLong(2000 * sizeof(int) * 10, 80000);
        newarena(b, abuf, 80000);

        print_arena_head();
        RANGE(i, 2000) { b.RealMake<int>(10); }
    }

    TEST_RESULTS();
}
