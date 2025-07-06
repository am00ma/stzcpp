# Slice

## API

```cpp
template <typename T> struct Slice {
    T*    data;
    isize len;
    isize cap;

    // Lifetimes
    Slice() = default;
    Slice(T* data_, isize len_, isize cap_)
    Slice(Arena* a, isize cap_)

    // Canonical way to fill up slice
    void Append(T val);

    // Get ith item
    T* operator[](isize i);

    // Get (i - j)th item
    Slice<T> operator[](isize i, isize j);

    // Get (i - j)th item as copy
    Slice<T> operator[](isize i, isize j, Arena* a);

    // str_equal
    b32 operator==(Slice<T> s);
};
```

## Usage

## Tests
