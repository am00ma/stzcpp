# Types

## Imports

```cpp
#include <cstddef>
#include <cstdint>
#include <cstring>
```
## Types

```cpp
typedef uint8_t   u8;
typedef int32_t   b32;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef char16_t  c16;
typedef float     f32;
typedef double    f64;
typedef uintptr_t uptr;
typedef ptrdiff_t isize;
typedef size_t    usize;

```

## Result

```cpp
template <typename T, typename E> struct Result {
    T data;
    E err;
};
```
