# Buf

## Includes

```cpp
--8<-- "docs/code/include/buf.h:3:4"
```

## Fields

```cpp
--8<-- "docs/code/include/buf.h:8:10"
```

## Constructors

```cpp
--8<-- "docs/code/include/buf.h:12:20"
```

## Destructors

```cpp
--8<-- "docs/code/include/buf.h:22:29"
```

## Operators

```cpp
--8<-- "docs/code/include/buf.h:31:71"
```

## Methods

```cpp
--8<-- "docs/code/include/buf.h:73:84"
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

|     | Case                                         | Correct | Total |
| --- | -------------------------------------------- | ------- | ----- |
| 1   | Stuct size                                   | 1       | 1     |
| 2   | Initialization                               | 6       | 6     |
| 3   | Usage: Use full maxlen                       | 2       | 2     |
| 4   | Usage: Use temp buffer to optimize mem usage | 2       | 2     |
| ✔  | 4 / 4 cases                                  |         |

```cpp
--8<-- "docs/code/tests/test_buf.cpp"
```
