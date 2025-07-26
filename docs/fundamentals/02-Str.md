# Str

## Includes

```cpp
--8<-- "docs/code/include/str.h:3:4"
```

## Helpers

```cpp
--8<-- "docs/code/include/str.h:10:23"
```

## Fields

```cpp
--8<-- "docs/code/include/str.h:30:34"
```

## Constructors

```cpp
--8<-- "docs/code/include/str.h:40:91"
```

## Operators

```cpp
--8<-- "docs/code/include/str.h:97:116"
```

## Memory, interface with C strings

```cpp
--8<-- "docs/code/include/str.h:121:150"
```

## Split

BUG: Needs to be defined outside of struct.

Declaration:

```cpp
--8<-- "docs/code/include/str.h:152:153"
```

Implementation:

```cpp
--8<-- "docs/code/include/str.h:243:288"
```

## Strs

### Fields

```cpp
--8<-- "docs/code/include/str.h:182:183"
```

### Initialization

```cpp
--8<-- "docs/code/include/str.h:185:197"
```

### Operators

```cpp
--8<-- "docs/code/include/str.h:199:235"
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
