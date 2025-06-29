# Zig Standard Library

- [std - Zig Documentation](https://ziglang.org/documentation/master/std/)

## struct

## Types

1.  ArrayHashMap
2.  ArrayHashMapUnmanaged
3.  ArrayList
4.  ArrayListAligned
5.  ArrayListAlignedUnmanaged
6.  ArrayListUnmanaged
7.  AutoArrayHashMap
8.  AutoArrayHashMapUnmanaged
9.  AutoHashMap
10. AutoHashMapUnmanaged
11. BitStack
12. BoundedArray
13. BoundedArrayAligned
14. BufMap
15. BufSet
16. Build
17. DoublyLinkedList
18. DynLib
19. DynamicBitSet
20. DynamicBitSetUnmanaged
21. EnumArray
22. EnumMap
23. EnumSet
24. HashMap
25. HashMapUnmanaged
26. MultiArrayList
27. Options
28. PriorityDequeue
29. PriorityQueue
30. Progress
31. Random
32. RingBuffer
33. SegmentedList
34. SemanticVersion
35. SinglyLinkedList
36. StaticBitSet
37. StaticStringMap
38. StaticStringMapWithEql
39. StringArrayHashMap
40. StringArrayHashMapUnmanaged
41. StringHashMap
42. StringHashMapUnmanaged
43. Target
44. Thread
45. Treap
46. Tz
47. Uri

## Namespaces

1. array_hash_map
2. ascii
3. atomic
4. base64
5. bit_set
6. builtin
7. c
8. coff
9. compress
10. crypto
11. debug
12. dwarf
13. elf
14. enums
15. fifo
16. fmt
17. fs
18. gpu
19. hash
20. hash_map
21. heap
22. http
23. io
24. json
25. leb
26. log
27. macho
28. math
29. mem
30. meta
31. net
32. os
33. pdb
34. pie
35. posix
36. process
37. simd
38. sort
39. start
40. static_string_map
41. tar
42. testing
43. time
44. tz
45. unicode
46. valgrind
47. wasm
48. zig
49. zip
50. zon

## Values

options Options: Stdlib-wide options that can be overridden by the root file.

## Functions

1. once

```zig
pub fn once(comptime f: fn () void) Once(f)
```
