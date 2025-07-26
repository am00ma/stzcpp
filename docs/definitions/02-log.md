# Log

## Pretty output

### Colors

```cpp
--8<-- "docs/code/include/log.h:6:51"
```

### title(...)

Bold blue text with format string.

```cpp
--8<-- "docs/code/include/log.h:53:55"
```

### debug(...)

Blue text with format string.

```cpp
--8<-- "docs/code/include/log.h:56:56"
```

### error(...)

Red text, `[E]` prefix, format string after

```cpp
--8<-- "docs/code/include/log.h:57:57"
```

## Error handling

### Fatal(err, ...)

Crash entirely

```cpp
--8<-- "docs/code/include/log.h:64:71"
```

### CheckErr(err, ...)

Print error but continue

```cpp
--8<-- "docs/code/include/log.h:73:79"
```

### Return(err, ...)

Print error and return to parent

```cpp
--8<-- "docs/code/include/log.h:81:88"
```

### Assert(err, ...)

Debugger friendly assert

- [nullprogram: Assertions should be more debugger-oriented](https://nullprogram.com/blog/2022/06/26/)

```cpp
--8<-- "docs/code/include/log.h:90:97"
```
