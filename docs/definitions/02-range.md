# Range

## Range over loops

```cpp
#define RANGE(...) RANGEx(__VA_ARGS__, RANGE4, RANGE3, RANGE2, RANGE1)(__VA_ARGS__)
#define RANGEx(a, b, c, d, e, ...) e

#define RANGE1(i)          for (isize i = 0  ; i < 1  ; i++)
#define RANGE2(i, b)       for (isize i = 0  ; i < (b); i++)
#define RANGE3(i, a, b)    for (isize i = (a); i < (b); i++)
#define RANGE4(i, a, b, c) for (isize i = (a); i < (b); i += (c))
```

## Usage

```cpp
--8<-- "docs/code/examples/definitions/example_range_1.cpp"
```
