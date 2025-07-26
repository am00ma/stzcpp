# Trie

> Source: [Hash trie](https://nullprogram.com/blog/2023/09/30/)

TODO: Get rid of 'Owning/Non-owning' stuff, that can be handled outside.

## Includes

```cpp
--8<-- "docs/code/include/ltrie.h:3:3"
```

## LTrieChild

### Fields

```cpp
--8<-- "docs/code/include/ltrie.h:7:10"
```

### Methods

```cpp
--8<-- "docs/code/include/ltrie.h:12:20"
```

## LTrie

Root struct to hold `LTrieChild`.

## Fields

```cpp
--8<-- "docs/code/include/ltrie.h:25:30"
```

## Constructors

```cpp
--8<-- "docs/code/include/ltrie.h:36:43"
```

## Destructors

None

## Operators

```cpp
--8<-- "docs/code/include/ltrie.h:49:73"
```

## Methods

```cpp
--8<-- "docs/code/include/ltrie.h:75:84"
```

## Usage

| no  | desc |
| --- | ---- |
|     |      |

## Tests

|     | Case                                            | Correct | Total |
| --- | ----------------------------------------------- | ------- | ----- |
| 1   | Initialization                                  | 0       | 0     |
| 2   | Usage: Typical usage                            | 3       | 3     |
| 3   | Insert, Lookup: Successful, unsuccessful lookup | 20      | 20    |
| 4   | Iteration: Typical usage                        | 20      | 20    |
| ✔  | 4 / 4 cases                                     |         |       |

```cpp
--8<-- "docs/code/tests/test_ltrie.cpp"
```
