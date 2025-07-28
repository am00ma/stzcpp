# Arena

## TODO: Usage

Examples:

| no  | desc                                                                     |
| --- | ------------------------------------------------------------------------ |
| 1   | Return valid reference from function                                     |
| 2   | Nested structs that need initialization                                  |
| 3   | Read files and reclaim space on error                                    |
| 4   | Parsing a file, and growing an array dynamically                         |
| 5   | Growing two arrays dynamically                                           |
| 6   | Using a temporary arena per frame (e.g. to allocate on realtime threads) |
| 7   | Using threads to load files async                                        |
| 8   | Aligned variable length structs                                          |

## API

```cpp

typedef struct Arena{

    // Fields
    char* beg = 0;
    char* end = 0;
    isize cap = 0;

    // Lifetime:
    Arena();                                      // Zero init?
    Arena(isize cap_);                            // Given capacity, uses malloc
    Arena(isize cap_, Arena* src, b32 flags = 0); // From another arena
    Arena(char* buf, isize cap_);                 // From fields

    void Free();

    // Operators:

    // Methods:
    isize Used();
    isize Available<V>();
    T*    Make<V>(isize count = 1, b32 flags = 0, A... args);

    // Debugging:
    void  Print();

}

// Arena on stack, exact behaviour as on heap
#define BufArena(a, buf, cap)                                                                                          \
    char  buf[cap] = {};                                                                                               \
    Arena a        = Arena(buf, cap);

```

## Code

### Includes

How to deal with system includes?

- `cstdio`
- `cstdlib`
- `new`

```cpp
#include "log.h"   // Fatal, debug
#include "range.h" // RANGE
#include "types.h" // isize, b32

#include <cstdio>  // printf
#include <cstdlib> // malloc, free
#include <new>     // Needed for proper behaviour of `new` in `Make` !!
```

### Fields

Total size: 24 bytes (3 \* 64bits)

```cpp
char* beg = 0;
char* end = 0;
isize cap = 0;
```

### Lifetime

- Zero init?
- Given capacity, uses malloc
- From another arena
- From fields

```cpp
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
```

From `char[]` on stack:

```cpp
// Arena on stack, exact behaviour as on heap
#define BufArena(a, buf, cap)                                                                                          \
    char  buf[cap] = {};                                                                                               \
    Arena a        = Arena(buf, cap);
```

### Destruction

Rare case that requires `Free:`

```cpp
// No free by destructor
//    ( as only applicable if it was allocated
//      with malloc constructor: Arena(isizze cap_) )
void Free()
{
    if (end - cap) free(end - cap);
}
```

### Methods

Checking used space and available space for given type:

```cpp
isize Used() { return cap - (end - beg); }

// How many objects can be allocated?
// TODO: Check floor, ceil, padding
template <typename T, typename... A> isize Available()
{
    isize pad = -(uptr)beg & (alignof(T) - 1);
    return (end - beg - pad) / sizeof(T);
}
```

Usage with support for defaults and typing:

```cpp

typedef enum {

    NOZERO   = 0x1,
    SOFTFAIL = 0x2,
    DEFAULTS = 0x4,

} ArenaFlags;

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
```

### Debugging

```cpp
void Print(const char* label)
{
    debug("[A] %s: used: %ld / cap: %ld (beg: %p, end: %p)\n", //
            label, cap - (end - beg), cap, (void*)beg, (void*)end);
}
```

## Tests

| No. | Case                                      | Correct | Total |
| --- | ----------------------------------------- | ------- | ----- |
| 1   | Deps: alignof, sizeof                     | 26      | 26    |
| 2   | Size: struct                              | 1       | 1     |
| 3   | malloc up to 2^35, without free           | 63      | 63    |
| 4   | malloc up to 2^35, with free              | 35      | 35    |
| 5   | Allocated sizes                           | 1       | 1     |
| 6   | Zeroed Initialization for primitives      | 3       | 3     |
| 7   | Zeroed Initialization for structs         | 6       | 6     |
| 8   | Elements with defaults                    | 6       | 6     |
| 9   | Elements with default args, but overriden | 6       | 6     |
| 10  | Non-zeroed Initialization                 | 6       | 6     |
| 11  | Zeroed Initialization                     | 6       | 6     |
| 12  | Soft-fail                                 | 1       | 1     |
| 13  | TODO: Non-aligned access                  | 1       | 1     |
| 14  | TODO: Multiple threads                    | 1       | 1     |
| --- | ----                                      | ------- | ----- |
| ✔  | Cases: 14 / 14                            | 162     | 162   |

Arena Tests (Use `ctrl+f` to navigate to code)

### 1. Deps: alignof, sizeof

| type                                  | sizeof | alignof | comment           |
| ------------------------------------- | ------ | ------- | ----------------- |
| char                                  | 1      | 1       | Expected          |
| u16                                   | 2      | 2       | Expected          |
| u32                                   | 4      | 4       | Expected          |
| u64                                   | 8      | 8       | Expected          |
| i16                                   | 2      | 2       | Expected          |
| i32                                   | 4      | 4       | Expected          |
| i64                                   | 8      | 8       | Expected          |
| `struct s1 { u16 a, b, c; }`          | 6      | 2       | Ok                |
| `struct s5 {i32 a; i16 b;}`           | 8      | 4       | _why != 6?_       |
| `struct s2 { u16 a, b, c; char d[] }` | 6      | 2       | `d[]` not counted |
| `<char> struct s3 {u32 a; V e[];}`    | 4      | 4       | `e[]` not counted |
| `<i16> struct s3 {u32 a; V e[];}`     | 4      | 4       | `e[]` not counted |
| `<i16> struct s4 {u32 a; V e[3];}`    | 12     | 4       | _why 12?_         |

```cpp
TEST_CASE("Deps: alignof, sizeof")
{
    TEqual(sizeof(char), (isize)1, "%ld");
    TEqual(alignof(char), (isize)1, "%ld");

    TEqual(sizeof(u16), (isize)2, "%ld");
    TEqual(alignof(u16), (isize)2, "%ld");

    TEqual(sizeof(u32), (isize)4, "%ld");
    TEqual(alignof(u32), (isize)4, "%ld");

    TEqual(sizeof(u64), (isize)8, "%ld");
    TEqual(alignof(u64), (isize)8, "%ld");

    TEqual(sizeof(i16), (isize)2, "%ld");
    TEqual(alignof(i16), (isize)2, "%ld");

    TEqual(sizeof(i32), (isize)4, "%ld");
    TEqual(alignof(i32), (isize)4, "%ld");

    TEqual(sizeof(i64), (isize)8, "%ld");
    TEqual(alignof(i64), (isize)8, "%ld");

    struct s1 {
        u16 a, b, c;
    };

    TEqual(sizeof(s1), (isize)6, "%ld");
    TEqual(alignof(s1), (isize)2, "%ld");

    struct s2 {
        u16  a, b, c;
        char d[];
    };

    TEqual(sizeof(s2), (isize)6, "%ld");
    TEqual(alignof(s2), (isize)2, "%ld");

    // template <typename V> struct s3 {
    //     u32  a;
    //     V    e[];
    // };
    // --> Errors with
    // Flexible array member 'd' with type 'char[]' is not at the end of struct [flexible_array_not_at_end]

    // template <typename V> struct s3 {
    //     u32  a;
    //     V    e[];
    // };
    // --> Has to be defined in global namespace?
    // 1. Templates can only be declared in namespace or class scope [template_outside_namespace_or_class_scope]
    TEqual(sizeof(s3<char>), (isize)4, "%ld");
    TEqual(alignof(s3<char>), (isize)4, "%ld");

    // Passes, as V[] is not included in size of struct
    TEqual(sizeof(s3<i16>), (isize)4, "%ld");
    TEqual(alignof(s3<i16>), (isize)4, "%ld");

    // template <typename V> struct s4 {
    //     i32 a;
    //     V   e[3];
    // };
    // Passes, as V[] is not included in size of struct
    TEqual(sizeof(s4<i16>), (isize)12, "%ld"); // (i32 = 4) + (V* = 8)
    TEqual(alignof(s4<i16>), (isize)4, "%ld");

    struct s5 {
        i32 a;
        i16 b;
    };

    // NOTE: is not equal to 6 -> (i32 = 4) + (i16 = 2)
    TEqual(sizeof(s5), (isize)8, "%ld");
    TEqual(alignof(s5), (isize)4, "%ld");
}
```

### 2. Size: struct

```cpp
TEST_CASE("Size: struct")
{
    // The struct itself is very light, with 3 64-bit integers
    isize size = 24;
    TEqual(sizeof(Arena), size, "%ld"); // 8(beg) + 8(end) + 8(cap)
}
```

### 3. Malloc: up to 2^35, without free

Not sure why this behaviour occurs.

```cpp
TEST_CASE("Malloc: up to 2^35, without free")
{
    // It is possible to allot astonishing amount of memory,
    // all the way upto 4 EB (MB, GB, TB, PB, EB), that too, cumulatively
    // BUG: What is happening here? Does'nt reflect in btop
    isize cap = 1;
    RANGE(i, 63)
    {
        Arena a = {cap};
        TEqual(a.cap, cap, "%ld");

        cap *= 2;
    }
}
```

### 4. Malloc: up to 2^35, with free

Not sure why this behaviour occurs.

```cpp
TEST_CASE("Malloc: up to 2^35, with free")
{
    // BUG: If free is added, crashes at i=35 (32 GB)
    isize cap = 1;
    RANGE(i, 35)
    {
        Arena a = {cap};
        TEqual(a.cap, cap, "%ld");
        a.Free();

        cap *= 2;
    }
}
```

### 5. Alloc: Allocated sizes (well aligned)

```cpp
TEST_CASE("Alloc: Allocated sizes (well aligned)")
{
    Arena a = perm;
    i32*  x = a.Make<i32>();
    TEqual(a.Used(), (isize)4, "%ld");
}
```

### 6. Alloc: Allocated sizes (half aligned)

```cpp
TEST_CASE("Alloc: Allocated sizes (half aligned)")
{
    // NOTE: only 2 used
    Arena a = perm;
    i16*  x = a.Make<i16>();
    TEqual(a.Used(), (isize)2, "%ld");
}
```

### 7. Zeroed Initialization for primitives

```cpp
TEST_CASE("Alloc: Zeroed for primitives")
{
    Arena a = perm;
    i32*  y = a.Make<i32>(3);
    RANGE(i, 3) { TEqual(y[i], 0, "%d"); }
}
```

### 8. Zeroed Initialization for structs

Henceforth, using the following struct definition:

```cpp
// Arbitrary datatype
typedef struct Item {
    i32 a = 4;
    i32 b = 8;
} Item;
```

```cpp
TEST_CASE("Alloc: Zeroed for structs")
{
    Arena a = perm;
    Item* y = a.Make<Item>(3);
    RANGE(i, 3)
    {
        TEqual(y[i].a, 0, "%d");
        TEqual(y[i].b, 0, "%d");
    }
}
```

### 9. Elements with defaults

```cpp
TEST_CASE("Alloc: Elements with defaults")
{
    Arena a = perm;
    Item* y = a.Make<Item>(3, DEFAULTS);
    RANGE(i, 3)
    {
        TEqual(y[i].a, 4, "%d");
        TEqual(y[i].b, 8, "%d");
    }
}
```

### 10. Elements with default args, but overriden

```cpp
TEST_CASE("Alloc: Elements with overriden defaults")
{
    Arena a = perm;
    Item* y = a.Make<Item>(3, DEFAULTS, 3, 5); // Have to get order of new correct
    RANGE(i, 3)
    {
        TEqual(y[i].a, 3, "%d");
        TEqual(y[i].b, 5, "%d");
    }
}
```

### 11. Non-zeroed Initialization

```cpp
TEST_CASE("Alloc: Non-zeroed")
{
    Arena a = perm;

    // Values to overwrite
    Arena b   = a;
    Item* tmp = a.Make<Item>(3, DEFAULTS);

    // No memset
    Item* y = a.Make<Item>(3, NOZERO);
    RANGE(i, 3)
    {
        // Returns results from prev arena,
        // so we can assert the value
        TEqual(y[i].a, 4, "%d");
        TEqual(y[i].b, 8, "%d");
    }
}
```

### 12. Soft-fail

```cpp
TEST_CASE("Alloc: Soft-fail")
{
    Arena a = perm;
    Item* y = a.Make<Item>(1024, SOFTFAIL);
    TEqual(y, (Item*)0, "%p");
}
```

### 13. TODO: Non-aligned access

```cpp

```

### 14. TODO: Multiple threads

```cpp

```
