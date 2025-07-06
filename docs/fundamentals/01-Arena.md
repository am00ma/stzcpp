# Arena

## API

```cpp
typedef struct Arena {
    char* beg = 0;
    char* end = 0;
    isize cap = 0;

    // Default constructor
    Arena() = default;

    // Lifetime
    Arena(isize cap_);
    Arena(isize cap_, Arena* src);

    // Rarely used
    void  Free();
    char* OrigBeg();

    // Meat
    char* Alloc(isize objsize, isize align, isize count, b32 flags);

    // Syntactic sugar (args can be used to default initialize)
    T* Make(isize count = 1, b32 flags = 0, A... args);

    // Debug
    void Print(const char* label);

} Arena;
```

## Usage

```cpp
typedef struct Item {
    i32 a = 4;
    i32 b = 8;
} Item;

Arena a = Arena(1024);                // 1024 bytes
Item* x = a.Make<Item>(10);           // Default inits
RANGE(i, 10) { assert(x[i].a == 4); } // Checks
```

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
