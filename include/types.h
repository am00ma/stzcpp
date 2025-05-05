#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

/* ---------------------------------------------------------------------------
 * Types
 * ------------------------------------------------------------------------- */
typedef uint8_t   u8;
typedef int32_t   b32;
typedef int32_t   i32;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef char16_t  c16;
typedef float     f32;
typedef double    f64;
typedef uintptr_t uptr;
typedef ptrdiff_t isize;
typedef size_t    usize;

/* ---------------------------------------------------------------------------
 * Result
 * ------------------------------------------------------------------------- */
template <typename T> struct Result {
    T   res;
    int err;
};
