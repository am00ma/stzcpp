# Slice

## Includes

```cpp
--8<-- "docs/code/include/slice.h:3:4"
```

## Fields

```cpp
--8<-- "docs/code/include/slice.h:8:10"
```

## Constructors

```cpp
--8<-- "docs/code/include/slice.h:12:26"
```

## Destructors

None

## Operators

```cpp
--8<-- "docs/code/include/slice.h:28:77"
```

## Methods

```cpp
--8<-- "docs/code/include/slice.h:79:88"
```

## Usage

| no  | desc |
| --- | ---- |
|     |      |

## Tests

|     | Case                   | Correct | Total |
| --- | ---------------------- | ------- | ----- |
| 1.  | Stuct size             | 2       | 2     |
| 2.  | Append                 | 2       | 2     |
| 3.  | Item: By reference     | 1       | 1     |
| 4.  | Subslice: By reference | 4       | 4     |
| 5.  | Subslice: By reference | 4       | 4     |
| ✔  | 5 / 5 cases            |         |       |

```cpp
--8<-- "docs/code/tests/test_slice.cpp"
```
