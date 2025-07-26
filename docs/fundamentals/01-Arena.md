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

## Tests

Arena

|     | Case                                 | Correct | Total |
| --- | ------------------------------------ | ------- | ----- |
| 1.  | Stuct size                           | 1       | 1     |
| 2.  | malloc up to 2^35, without free      | 63      | 63    |
| 3.  | malloc up to 2^35, with free         | 35      | 35    |
| 4.  | Allocated sizes                      | 1       | 1     |
| 5.  | Zeroed Initialization for primitives | 3       | 3     |
| 6.  | Zeroed Initialization for structs    | 6       | 6     |
| 7.  | Elements with defaults               | 6       | 6     |
| 8.  | Elements with default args           | 6       | 6     |
| 9.  | Non-zeroed Initialization            | 6       | 6     |
| 10. | Zeroed Initialization                | 6       | 6     |
| 11. | Soft-fail                            | 1       | 1     |
| 12. | TODO: Non-aligned access             | 1       | 1     |
| 13. | TODO: Multiple threads               | 1       | 1     |

```cpp
--8<-- "docs/code/tests/test_arena.cpp"
```

## Usage

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
