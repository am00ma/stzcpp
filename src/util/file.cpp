#include "util/file.h"

#include <cstdio> // FILE, fopen, ...

FileResult File_Read(Arena* a, Str path)
{
    Arena temp      = *a;
    char* path_cstr = path.Cstr(&temp); // We dont need path after loading buf

    // Open and get size
    FILE* f = fopen(path_cstr, "rb");
    if (f == NULL) { return {"", FILE_FAIL_OPEN}; }
    if (fseek(f, 0, SEEK_END) != 0) { return {"", FILE_FAIL_SEEK}; }
    isize size = ftell(f);

    // Read data and cole
    Str data = {a, size};
    if (fseek(f, 0, SEEK_SET) != 0) { return {"", FILE_FAIL_SEEK}; }
    if (fread(data.buf, sizeof(char), data.len, f) != (usize)data.len) { return {"", FILE_FAIL_READ}; }
    if (fclose(f) != 0) { return {data, FILE_FAIL_CLOSE}; }

    return {data, FILE_SUCCESS};
}

FileResult File_Write(Arena* a, Str path, Str text, const char* mode)
{
    Arena temp      = *a;
    char* path_cstr = path.Cstr(&temp); // We dont need path after loading buf

    // Write to file (overwrite)
    FILE* f = fopen(path_cstr, mode);
    if (f == NULL) { return {"", FILE_FAIL_OPEN}; }
    if (fwrite(text.buf, sizeof(char), text.len, f) != (usize)text.len) { return {"", FILE_FAIL_WRITE}; }
    if (fclose(f) != 0) { return {text, FILE_FAIL_CLOSE}; }

    return {text, FILE_SUCCESS};
}
