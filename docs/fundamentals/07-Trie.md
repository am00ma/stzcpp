# Trie

>  Source: [Hash trie](https://nullprogram.com/blog/2023/09/30/)

## API

```cpp
template <typename V> struct Trie {

    Trie<V>* child[4] = {};

    Str key = ""; // Also gravestone?
    V   val = 0;  // Do we need a pointer?

    V* Insert(Str key, Arena* a, bool store_key = false);
    V* Lookup(Str key);

    V* operator[](Str key, Arena* a = 0, bool store_key = false);

    // Debugging
    Str* OrigKey(Str key);
}
```

## Usage

```cpp

```

## Tests

```cpp

```
