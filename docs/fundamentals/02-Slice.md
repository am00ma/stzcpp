# Slice

Notes:

1.  `Slice(T* buf, isize len_);` is convinience to set len equal to cap on init
2.  `Slice<T> Final(Arena* a);` is tricky but very useful
3.  `cap` is needed only for debugging and `Free`

```cpp
template <typename T> struct Slice {

    // Fields:
    T*    buf = 0;
    isize len = 0;
    isize cap = 0;

    // Lifetime:
    Slice();
    Slice(T* buf, isize len_, isize cap_);               // From buf
    Slice(T* buf, isize len_);                           // len = cap
    Slice(Arena* a, isize cap_);                         // On arena
    template <isize N> constexpr Slice(T (&s)[N]);       // From literals

    Slice<T> Final(Arena* a); // Use with caution

    // Operators:
    Slice<T>* operator+ (T val);                      // Append
    T*        operator[](isize i);                    // By reference
    Slice<T>  operator[](isize i, isize j);           // By reference
    Slice<T>  operator[](isize i, isize j, Arena* a); // By copy
    bool      operator==(Slice<T> s);                 // Equality

    // Methods:
    Slice<T> Copy(Arena* a); // To be tested
    Slice<T>* Append(T val); // Identical to operator+

    // Debugging:
}
```

## Includes

```cpp
--8<-- "docs/code/include/slice.h:3:4"
```

## Fields

```cpp
--8<-- "docs/code/include/slice.h:8:10"
```

## Constructors

```cpp
--8<-- "docs/code/include/slice.h:12:26"
```

## Destructors

None

## Operators

```cpp
--8<-- "docs/code/include/slice.h:28:77"
```

## Methods

```cpp
--8<-- "docs/code/include/slice.h:79:88"
```

## Usage

| no  | desc |
| --- | ---- |
|     |      |

## Tests

|     | Case                   | Correct | Total |
| --- | ---------------------- | ------- | ----- |
| 1.  | Stuct size             | 2       | 2     |
| 2.  | Append                 | 2       | 2     |
| 3.  | Item: By reference     | 1       | 1     |
| 4.  | Subslice: By reference | 4       | 4     |
| 5.  | Subslice: By reference | 4       | 4     |
| ✔  | 5 / 5 cases            |         |       |

```cpp
--8<-- "docs/code/tests/test_slice.cpp"
```
