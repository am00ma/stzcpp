# Str

## API

```cpp
typedef struct Str {
    // Hack for holding string literals as well
    union {
        char*       buf = 0;
        const char* cbuf;
    };
    isize len = 0;

    // Default constructor
    Str() = default;

    // From string literals
    template <isize N> constexpr Str(const char (&s)[N]);

    // From const
    Str(const char* buf_);

    // From fields
    Str(char* buf_, isize len_);

    // From Arena
    Str(Arena* a, isize len_);

    // From formatted string
    Str(Arena* a, isize len, char const* fmt, ...);

    // From spans
    Str(char* beg, char* end);

    // Get ith char
    char& operator[](isize i);

    // Get slice
    Str operator[](isize beg, isize end);

    // str_equal
    bool operator==(Str s);

    // Null terminated string
    char* Cstr(Arena* a);

    // Copy to arena
    Str Copy(Arena* a, bool null_terminate = false);

    // Split (defaults to splitting lines)
    Strs Split(Arena* a, Str delimiter = "\n", bool ignore_empty = true, bool substitute_null = false);

    // FNV hash
    u64 Hash64();

} Str;
```

## Usage

| no  | desc                                                                                                   |
| --- | ------------------------------------------------------------------------------------------------------ |
| 1   | Return string from function, get null terminated string                                                |
| 2   | Accept string and return parsed, formatted and contatenated string                                     |
| 3   | Accept string and return parsed, formatted and contatenated string, but using buffers to reclaim space |

## Tests

1. Initialization
2. Used memory check
3. Byte-wise memory check
4. Out of memory errors
5. Const buf reuse
