# Log

## Pretty output

### Colors

```cpp
--8<-- "docs/code/include/log.h:6:13"
```

### title(...)

Bold blue text with format string.

```cpp
--8<-- "docs/code/include/log.h:15:15"
```

### debug(...)

Blue text with format string.

```cpp
--8<-- "docs/code/include/log.h:16:16"
```

### error(...)

Red text, `[E]` prefix, format string after

```cpp
--8<-- "docs/code/include/log.h:17:17"
```

## Error handling

### Fatal(err, ...)

Crash entirely

```cpp
--8<-- "docs/code/include/log.h:23:29"
```

### CheckErr(err, ...)

Print error but continue

```cpp
--8<-- "docs/code/include/log.h:32:37"
```

### Return(err, ...)

Print error and return to parent

```cpp
--8<-- "docs/code/include/log.h:40:46"
```

### Assert(err, ...)

Debugger friendly assert

- [nullprogram: Assertions should be more debugger-oriented](https://nullprogram.com/blog/2022/06/26/)

```cpp
--8<-- "docs/code/include/log.h:49:55"
```
