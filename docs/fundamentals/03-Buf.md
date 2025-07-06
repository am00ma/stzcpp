# Buf

## API

```cpp
typedef struct Buf {
    char* buf;
    isize len;
    isize cap;

    Buf(Arena* a, isize cap_);

    Str Join(Str c);
    Str Final(Arena* a); // Shrinks arena
} Buf;
```

## Usage

| no  | desc                                                                                                   |
| --- | ------------------------------------------------------------------------------------------------------ |
| 1   | Accept string and return parsed, formatted and contatenated string, but using buffers to reclaim space |

## Tests

1. Initialization
2. Used memory check
3. Byte-wise memory check
4. Out of memory errors
5. Const buf reuse
6. Shrinking arena in invalid conditions
