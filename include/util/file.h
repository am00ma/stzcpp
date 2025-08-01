#pragma once

#include "str.h"

typedef enum {
    FILE_SUCCESS = 0,
    FILE_FAIL_OPEN,
    FILE_FAIL_SEEK,
    FILE_FAIL_CLOSE,
    FILE_FAIL_READ,
    FILE_FAIL_WRITE,
} FileError;

typedef Result<Str, FileError> FileResult;

FileResult File_Read(Arena* a, Str path);
FileResult File_Write(Arena* a, Str path, Str text);

template <typename T> Result<T*, FileError> Struct_Read(Arena* a, Str path)
{
    auto bytes_ok = File_Read(a, path);
    if (bytes_ok.err) return Result<T*, FileError>(0, FILE_FAIL_READ);
    if (bytes_ok.data.len != sizeof(T)) return Result<T*, FileError>(0, FILE_FAIL_READ);
    return Result<T*, FileError>((T*)bytes_ok.data.buf, FILE_SUCCESS);
}

template <typename T> Result<T*, FileError> Struct_Write(Arena* a, Str path, T* data)
{
    auto bytes_ok = File_Write(a, path, Str((char*)data, sizeof(T)));
    if (bytes_ok.err) return Result<T*, FileError>(0, FILE_FAIL_WRITE);
    if (bytes_ok.data.len != sizeof(T)) return Result<T*, FileError>(0, FILE_FAIL_WRITE);
    return Result<T*, FileError>((T*)bytes_ok.data.buf, FILE_SUCCESS);
}
