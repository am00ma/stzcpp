# Map

>  Source: [MSI map](https://nullprogram.com/blog/2022/08/08)

## API

```cpp
template <typename V> struct Map {
    Str*  keys    = 0; // Keys are strings as we need method `u64 Hash64()` and operator `==`
    V*    vals    = 0; // Any type
    isize cap_exp = 4; // Default to 16 elements
    isize len     = 0;

    Map(Arena* a, isize cap_exp_ = 4);

    V* Lookup(Str key);
    V* Insert(Str key);
}
```

## Usage

```cpp
TEST_CASE("Insert, Lookup: Successful")
{
    Arena a       = perm;
    isize cap_exp = 4;
    Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full

    Str key = "hello";
    Str val = "hi";

    *map.Insert(key) = val;

    Str* ret = map.Lookup(key);
    CHECK(*ret == val);
}

TEST_CASE("Lookup: Unsuccessful TODO: API is wild currently")
{
    Arena a       = perm;
    isize cap_exp = 4;
    Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full

    Str key = "hello";
    Str val = "hi";

    *map.Insert(key) = val;

    Str* ret = map.Lookup("how");
    CHECK(ret == 0);
}
```

## Tests

## Education:

1. Get the hash of key
2. Compute mask
3. Compute step
4. Iterate to compute index

```cpp
V* Lookup(Str key)
{
    u64 hash = key.Hash64(); // Hashing: O(n)
    u64 mask = (1 << cap_exp) - 1;
    u64 step = (hash >> (64 - cap_exp)) | 1;
    for (i64 i = hash;;)
    {
        i = (i + step) & mask;
        if (!keys[i].buf)
        {
            keys[i] = key;
            return &vals[i];
        }
        else if (keys[i] == key) { return &vals[i]; } // Equals: O(n)
    }
    return 0;
};
```

Example:

| var   | hex | decimal |
| ----- | --- | ------- |
| cap   | 10  | 16      |
| mask  | F   | -       |
| shift | -   | 60      |

| idx | key    | hash             | hash >> shift | step | step(decimal) | iterations for i              |
| --- | ------ | ---------------- | ------------- | ---- | ------------- | ----------------------------- |
| 0   | hello  | A430D84680AABD0B | 0A            | 0B   | 11            | 6, New                        |
| 1   | hi     | 08BA5F07B55EC3DA | 00            | 01   | 1             | 11, New                       |
| 2   | how    | 335F27192FF5D5ED | 03            | 03   | 3             | 0, New                        |
| 3   | are    | E756C8190570DA4D | 0E            | 0F   | 15            | 12, New                       |
| 4   | you    | B5617C19799EA15C | 0B            | 0B   | 11            | 7, New                        |
| 5   | hello1 | A9BCB9CCA21F898E | 0A            | 0B   | 11            | 9, New                        |
| 6   | hi1    | 337365193006E851 | 03            | 03   | 3             | 4, New                        |
| 7   | how1   | 40854BCC7EBA64D4 | 04            | 05   | 5             | 9, 14, New                    |
| 8   | are1   | 89507E843EC340B4 | 08            | 09   | 9             | 13, New                       |
| 9   | you1   | D3474C49A88C4C37 | 0D            | 0D   | 13            | 4, 1, New                     |
| 10  | hello2 | A9BCB8CCA21F87DB | 0A            | 0B   | 11            | 6, 1, 12, 7, 2, New           |
| 11  | hi2    | 337362193006E338 | 03            | 03   | 3             | 11, 14, 1, 4, 7, 10, New      |
| 12  | how2   | 40854ECC7EBA69ED | 04            | 05   | 5             | 2, 7, 12, 1, 6, 11, 0, 5, New |
| 13  | are2   | 895081843EC345CD | 08            | 09   | 9             | 6, 15, New                    |
| 14  | you2   | D3474D49A88C4DEA | 0D            | 0D   | 13            | 7, 4, 1, 14, 11, 8, New       |
| 15  | hello3 | A9BCB7CCA21F8628 | 0A            | 0B   | 11            | 3, New                        |

## Drawbacks

1. Single func for `Insert`, `Lookup` means we cannot keep track of length
2. If more than `cap` is inserted, goes into infinite loop
3. To avoid above, need to check for 0 each time we `Insert` or `Lookup`
4. Should we use fatten the struct with another `isize` to store `cap` to avoid `isize` mistakes (`(isize)1 << cap_exp`)?
