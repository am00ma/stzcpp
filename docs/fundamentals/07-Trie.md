# Trie

> Source: [Hash trie](https://nullprogram.com/blog/2023/09/30/)

TODO: Get rid of 'Owning/Non-owning' stuff, that can be handled outside.

## Includes

```cpp
--8<-- "docs/code/include/trie.h:3:3"
```

## Fields

```cpp
--8<-- "docs/code/include/trie.h:7:10"
```

## Constructors

```cpp
--8<-- "docs/code/include/trie.h:15:15"
```

## Destructors

None

## Operators

```cpp
--8<-- "docs/code/include/trie.h:21:46"
```

## Methods

```cpp
--8<-- "docs/code/include/trie.h:48:97"
```

## Usage

| no  | desc |
| --- | ---- |
|     |      |

## Tests

|     | Case                                           | Correct | Total |
| --- | ---------------------------------------------- | ------- | ----- |
| 1.  | Algorithm                                      | 17      | 17    |
| 2.  | Struct size                                    | 1       | 1     |
| 3.  | Initialization                                 | 2       | 2     |
| 4.  | Insert, Lookup: Successful                     | 1       | 1     |
| 5.  | Insert, Lookup: Unsuccessful                   | 1       | 1     |
| 6.  | Insert, Lookup, OrigKey: Non-owning keys       | 3       | 3     |
| 7.  | Insert, Lookup, OrigKey: Storing keys in arena | 3       | 3     |
| 8.  | Insert, Lookup: Operator                       | 3       | 3     |
| 9.  | Usage: Typical usage                           | 9       | 9     |
| 10. | Iteration: Typical usage                       | 20      | 20    |
| ✔  | 10 / 10 cases                                  |         |       |

```cpp
--8<-- "docs/code/tests/test_trie.cpp"
```
