# LTrie

>  Source: [Hash trie](https://nullprogram.com/blog/2023/09/30/)

## API

```cpp

// Helper - Actual Trie structure
template <typename V> struct LTrieChild {
    LTrieChild<V>* child[4] = {};

    Str key = "";
    V   val = 0;

    // Debugging
    void Print();
    void PrintChildren();
};

template <typename V> struct LTrie {
    LTrieChild<V> root = {};

    isize len = 0;
    isize cap = 0;

    LTrieChild<V>* data;

    // Initialization
    LTrie(Arena* a, isize cap_);

    // Only 'method'
    V* operator[](Str key, bool insert = false);

    // Debugging
    void Print(const char* label, bool children = false);
}
```

## Usage

```cpp
TEST_CASE("Usage: Typical usage (second arg is for insertion)")
{
    Arena a    = perm;
    STrie trie = STrie(&a, 25);
    Str*  ret  = {};

    *trie["hello", true] = "hi";

    ret = trie["hello"];
    CHECK(*ret == Str("hi"));

    *trie["how", true] = "are";

    ret = trie["how"];
    CHECK(*ret == Str("are"));

    *trie["you", true] = "";

    ret = trie["you"];
    CHECK(*ret == Str(""));
}
```

## Tests

```cpp

```
