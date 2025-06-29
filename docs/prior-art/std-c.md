# C Standard Library

- [cppreference](https://en.cppreference.com/w/c/header.html)

| no  | name            | desc                                                                                                    | since | deprecated |
| --- | --------------- | ------------------------------------------------------------------------------------------------------- | ----- | ---------- |
| 1.  | `stddef.h`      | Common macro definitions                                                                                |       |            |
| 2.  | `errno.h`       | Macros reporting error conditions                                                                       |       |            |
| 3.  | `assert.h`      | Conditionally compiled macro that compares its argument to zero                                         |       |            |
| 4.  | `setjmp.h`      | Nonlocal jumps                                                                                          |       |            |
| 5.  | `limits.h`      | Ranges of integer types                                                                                 |       |            |
| 6.  | `float.h`       | Limits of floating-point types                                                                          |       |            |
| 7.  | `math.h`        | Common mathematics functions                                                                            |       |            |
| 8.  | `ctype.h`       | Functions to determine the type contained in character data                                             |       |            |
| 9.  | `string.h`      | String handling                                                                                         |       |            |
| 10. | `time.h`        | Time/date utilities                                                                                     |       |            |
| 11. | `locale.h`      | Localization utilities                                                                                  |       |            |
| 12. | `stdarg.h`      | Variable arguments                                                                                      |       |            |
| 13. | `signal.h`      | Signal handling                                                                                         |       |            |
| 14. | `stdio.h`       | Input/output                                                                                            |       |            |
| 15. | `stdlib.h`      | General utilities: memory management, program utilities, string conversions, random numbers, algorithms |       |            |
| 16. | `iso646.h`      | Alternative operator spellings                                                                          | C95   |            |
| 17. | `wchar.h`       | Extended multibyte and wide character utilities                                                         | C95   |            |
| 18. | `wctype.h`      | Functions to determine the type contained in wide character data                                        | C95   |            |
| 19. | `inttypes.h`    | Format conversion of integer types                                                                      | C99   |            |
| 20. | `tgmath.h`      | Type-generic math (macros wrapping <math.h> and <complex.h>)                                            | C99   |            |
| 21. | `stdint.h`      | Fixed-width integer types                                                                               | C99   |            |
| 22. | `fenv.h`        | Floating-point environment                                                                              | C99   |            |
| 23. | `complex.h`     | Complex number arithmetic                                                                               | C99   |            |
| 24. | `stdbool.h`     | Macros for boolean type                                                                                 | C99   | C23        |
| 25. | `threads.h`     | Thread library                                                                                          | C11   |            |
| 26. | `uchar.h`       | UTF-16 and UTF-32 character utilities                                                                   | C11   |            |
| 27. | `stdatomic.h`   | Atomic operations                                                                                       | C11   |            |
| 28. | `stdnoreturn.h` | noreturn convenience macro                                                                              | C11   | C23        |
| 29. | `stdalign.h`    | alignas and alignof convenience macros                                                                  | C11   | C23        |
| 30. | `stdbit.h`      | Macros to work with the byte and bit representations of types                                           | C23   |            |
| 31. | `stdckdint.h`   | Macros for performing checked integer arithmetic                                                        | C23   |            |
| 32. | `stdmchar.h`    | Text transcode                                                                                          | C29   |            |

## Common

- `stddef.h`
- `errno.h`
- `assert.h`
- `setjmp.h`

## Math

- `limits.h`
- `float.h`
- `math.h`
- `inttypes.h`
- `tgmath.h`
- `fenv.h`
- `complex.h`
- `stdint.h`
- `stdbool.h`
- `stdbit.h`
- `stdckdint.h`

## String

- `ctype.h`
- `string.h`
- `time.h`
- `locale.h`
- `wchar.h`
- `wctype.h`
- `uchar.h`
- `stdmchar.h`

## Shell

- `stdarg.h`
- `signal.h`
- `stdio.h`

## Memory, Misc

- `stdlib.h`
- `stdnoreturn.h`
- `stdalign.h`
- `iso646.h`

## Concurrency

- `threads.h`
- `stdatomic.h`
