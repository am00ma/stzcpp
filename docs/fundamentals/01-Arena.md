# Arena

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

### 2. Size: struct

### 3. malloc up to 2^35, without free

### 4. malloc up to 2^35, with free

### 5. Allocated sizes

### 6. Zeroed Initialization for primitives

### 7. Zeroed Initialization for structs

### 8. Elements with defaults

### 9. Elements with default args, but overriden

### 10. Non-zeroed Initialization

### 11. Zeroed Initialization

### 12. Soft-fail

### 13. TODO: Non-aligned access

### 14. TODO: Multiple threads

```cpp
--8<-- "docs/code/tests/test_arena.cpp"
```

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

## Tests

1. Initialization
2. Used memory check
3. Byte-wise memory check
4. Out of memory errors
5. Aligned datatypes
6. Casting memory
7. Temporary arenas
8. Const buf arenas
