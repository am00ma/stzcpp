# File

## API

```cpp
typedef enum {
    FILE_SUCCESS = 0,
    FILE_FAIL_OPEN,
    FILE_FAIL_SEEK,
    FILE_FAIL_CLOSE,
    FILE_FAIL_READ,
    FILE_FAIL_WRITE,
} FileError;

Result<Str, FileError> File_Read(Arena* a, Str path);
Result<Str, FileError> File_Write(Arena* a, Str path, Str text);

template <typename T> Result<T*, FileError> Struct_Read(Arena* a, Str path);
template <typename T> Result<T*, FileError> Struct_Write(Arena* a, Str path, T* data);
```

## Usage

## Tests
