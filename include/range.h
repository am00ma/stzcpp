#pragma once

// clang-format off

#define FOR(...)                   FORx(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define FORx(a, b, c, d, e, ...)   e
#define FOR1(s)                    for (isize i = 0  ; i < (s).len; i++)
#define FOR2(s, i)                 for (isize i = 0  ; i < (s).len; i++)
#define FOR3(s, i, n)              for (isize i = 0  ; i < (s).n  ; i++)
#define FOR4(s, i, a, b)           for (isize i = (a); i < (b)    ; i++)

#define RANGE(...)                 RANGEx(__VA_ARGS__, RANGE4, RANGE3, RANGE2, RANGE1)(__VA_ARGS__)
#define RANGEx(a, b, c, d, e, ...) e
#define RANGE1(i)                  for (isize i = 0  ; i < 10 ; i++)
#define RANGE2(i, b)               for (isize i = 0  ; i < (b); i++)
#define RANGE3(i, a, b)            for (isize i = (a); i < (b); i++)
#define RANGE4(i, a, b, c)         for (isize i = (a); i < (b); i += (c))

// clang-format on
