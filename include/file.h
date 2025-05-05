#pragma once

#include "str.h"
#include "types.h"

typedef enum {
    FILE_SUCCESS = 0,
    FILE_FAIL_OPEN,
    FILE_FAIL_SEEK,
    FILE_FAIL_READ,
    FILE_FAIL_CLOSE,
} FileError;

Result<Str, FileError> File_Read(Arena* a, Str path);
