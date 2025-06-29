# Rust Standard Library

| no  | module                     | desc                                                                                                            |
| --- | -------------------------- | --------------------------------------------------------------------------------------------------------------- |
| 1.  | alloc                      | Memory allocation APIs.                                                                                         |
| 2.  | any                        | Utilities for dynamic typing or type reflection.                                                                |
| 3.  | arch                       | SIMD and vendor intrinsics module.                                                                              |
| 4.  | array                      | Utilities for the array primitive type.                                                                         |
| 5.  | ascii                      | Operations on ASCII strings and characters.                                                                     |
| 6.  | backtrace                  | Support for capturing a stack backtrace of an OS thread                                                         |
| 7.  | borrow                     | A module for working with borrowed data.                                                                        |
| 8.  | boxed                      | The Box<T> type for heap allocation.                                                                            |
| 9.  | cell                       | Shareable mutable containers.                                                                                   |
| 10. | char                       | Utilities for the char primitive type.                                                                          |
| 11. | clone                      | The Clone trait for types that cannot be ‘implicitly copied’.                                                   |
| 12. | cmp                        | Utilities for comparing and ordering values.                                                                    |
| 13. | collections                | Collection types.                                                                                               |
| 14. | convert                    | Traits for conversions between types.                                                                           |
| 15. | default                    | The Default trait for types with a default value.                                                               |
| 16. | env                        | Inspection and manipulation of the process’s environment.                                                       |
| 17. | error                      | Interfaces for working with Errors.                                                                             |
| 18. | f32                        | Constants for the f32 single-precision floating point type.                                                     |
| 19. | f64                        | Constants for the f64 double-precision floating point type.                                                     |
| 20. | ffi                        | Utilities related to FFI bindings.                                                                              |
| 21. | fmt                        | Utilities for formatting and printing Strings.                                                                  |
| 22. | fs                         | Filesystem manipulation operations.                                                                             |
| 23. | future                     | Asynchronous basic functionality.                                                                               |
| 24. | hash                       | Generic hashing support.                                                                                        |
| 25. | hint                       | Hints to compiler that affects how code should be emitted or optimized.                                         |
| 26. | i8Deprecation planned      | Redundant constants module for the i8 primitive type.                                                           |
| 27. | i16Deprecation planned     | Redundant constants module for the i16 primitive type.                                                          |
| 28. | i32Deprecation planned     | Redundant constants module for the i32 primitive type.                                                          |
| 29. | i64Deprecation planned     | Redundant constants module for the i64 primitive type.                                                          |
| 30. | i128Deprecation planned    | Redundant constants module for the i128 primitive type.                                                         |
| 31. | io                         | Traits, helpers, and type definitions for core I/O functionality.                                               |
| 32. | isizeDeprecation planned   | Redundant constants module for the isize primitive type.                                                        |
| 33. | iter                       | Composable external iteration.                                                                                  |
| 34. | marker                     | Primitive traits and types representing basic properties of types.                                              |
| 35. | mem                        | Basic functions for dealing with memory.                                                                        |
| 36. | net                        | Networking primitives for TCP/UDP communication.                                                                |
| 37. | num                        | Additional functionality for numerics.                                                                          |
| 38. | ops                        | Overloadable operators.                                                                                         |
| 39. | option                     | Optional values.                                                                                                |
| 40. | os                         | OS-specific functionality.                                                                                      |
| 41. | panic                      | Panic support in the standard library.                                                                          |
| 42. | path                       | Cross-platform path manipulation.                                                                               |
| 43. | pin                        | Types that pin data to a location in memory.                                                                    |
| 44. | prelude                    | The Rust Prelude                                                                                                |
| 45. | primitive                  | This module reexports the primitive types to allow usage that is not possibly shadowed by other declared types. |
| 46. | process                    | A module for working with processes.                                                                            |
| 47. | ptr                        | Manually manage memory through raw pointers.                                                                    |
| 48. | rc                         | Single-threaded reference-counting pointers. ‘Rc’ stands for ‘Reference Counted’.                               |
| 49. | result                     | Error handling with the Result type.                                                                            |
| 50. | slice                      | Utilities for the slice primitive type.                                                                         |
| 51. | str                        | Utilities for the str primitive type.                                                                           |
| 52. | string                     | A UTF-8–encoded, growable string.                                                                               |
| 53. | sync                       | Useful synchronization primitives.                                                                              |
| 54. | task                       | Types and Traits for working with asynchronous tasks.                                                           |
| 55. | thread                     | Native threads.                                                                                                 |
| 56. | time                       | Temporal quantification.                                                                                        |
| 57. | u8Deprecation planned      | Redundant constants module for the u8 primitive type.                                                           |
| 58. | u16Deprecation planned     | Redundant constants module for the u16 primitive type.                                                          |
| 59. | u32Deprecation planned     | Redundant constants module for the u32 primitive type.                                                          |
| 60. | u64Deprecation planned     | Redundant constants module for the u64 primitive type.                                                          |
| 61. | u128Deprecation planned    | Redundant constants module for the u128 primitive type.                                                         |
| 62. | usizeDeprecation planned   | Redundant constants module for the usize primitive type.                                                        |
| 63. | vec                        | A contiguous growable array type with heap-allocated contents, written Vec<T>.                                  |
| 64. | assert_matchesExperimental | Unstable module containing the unstable assert_matches macro.                                                   |
| 65. | async_iterExperimental     | Composable asynchronous iteration.                                                                              |
| 66. | autodiffExperimental       | This module provides support for automatic differentiation.                                                     |
| 67. | bstrExperimental           | The ByteStr and ByteString types and trait implementations.                                                     |
| 68. | f16Experimental            | Constants for the f16 half-precision floating point type.                                                       |
| 69. | f128Experimental           | Constants for the f128 quadruple-precision floating point type.                                                 |
| 70. | intrinsicsExperimental     | Compiler intrinsics.                                                                                            |
| 71. | patExperimental            | Helper module for exporting the pattern_type macro                                                              |
| 72. | randomExperimental         | Random value generation.                                                                                        |
| 73. | rangeExperimental          | Experimental replacement range types                                                                            |
| 74. | simdExperimental           | Portable SIMD module.                                                                                           |
| 75. | unsafe_binderExperimental  | Operators used to turn types into unsafe binders and back.                                                      |
