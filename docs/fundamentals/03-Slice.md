# List

Notes:

1.  `List(T* buf, isize len_);` is convinience to set len equal to cap on init
2.  `List<T> Final(Arena* a);` is tricky but very useful
3.  `cap` is needed only for debugging and `Free`

## TODO: Usage

Examples:

| no  | desc       |
| --- | ---------- |
| 1   | Table rows |

## API

```cpp
template <typename T> struct List {

    // Fields:
    union {
        T*       buf = 0;
        const T* cbuf;
    };
    isize len = 0;

    // Lifetime:
    List();
    List(T* buf, isize len_);                     // len = cap
    template <isize N> constexpr List(T (&s)[N]); // From literals
    List<T>(isize len_, Arena* a, b32 flags);     // Request on arena

    // Operators:
    T*      operator[](isize i);                    // By reference
    List<T> operator[](isize i, isize j);           // By reference
    List<T> operator[](isize i, isize j, Arena* a); // By copy
    bool    operator==(List<T> s);                  // Equality

    // Methods:
    List<T>  Copy(Arena* a); // To be tested

    // When paired with cap
    // isize cap = 0;
    // List(Arena* a, isize cap_);           // On arena
    // List(T* buf, isize len_, isize cap_); // From buf
    // List<T> Final(Arena* a);              // Use with caution
    // List<T>* operator+ (T val);           // Append
    // List<T>* Append(T val);               // Identical to operator+

    // Debugging:
};
```
