# Arena

## API

### Includes

```cpp
--8<-- "docs/code/include/arena.h:3:10"
```

### Fields

```cpp
--8<-- "docs/code/include/arena.h:25:27"
```

### Initialization

Flags:

```cpp
--8<-- "docs/code/include/arena.h:15:21"
```

From various memories:

```cpp
--8<-- "docs/code/include/arena.h:29:54"
```

From `char[]` on stack:

```cpp
--8<-- "docs/code/include/arena.h:115:117"
```

### Destruction

Rare case that requires `Free:`

```cpp
--8<-- "docs/code/include/arena.h:56:62"
```

### Methods

Allocation algorithm with padding computation:

```cpp
--8<-- "docs/code/include/arena.h:64:86"
```

Usage with support for defaults and typing:

```cpp
--8<-- "docs/code/include/arena.h:88:101"
```

### Debugging

```cpp
--8<-- "docs/code/include/arena.h:103:111"
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
