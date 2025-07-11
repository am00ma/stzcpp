#include "util/file.h"

#include <cstdio> // FILE, fopen, ...

Result<Str, FileError> File_Read(Arena* a, Str path)
{
    Arena temp      = *a;
    char* path_cstr = path.Cstr(&temp); // We dont need path after loading buf

    FILE* f = fopen(path_cstr, "rb");
    if (f == NULL)
    {
        perror("fopen");
        return Result<Str, FileError>(Str(""), FILE_FAIL_OPEN);
    }

    if (fseek(f, 0, SEEK_END) != 0)
    {
        perror("fseek");
        return Result<Str, FileError>(Str(""), FILE_FAIL_SEEK);
    }

    isize size = ftell(f);
    Str   data = Str(a->Make<char>(size), size);

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        perror("fseek");
        return Result<Str, FileError>(Str(""), FILE_FAIL_SEEK);
    }

    if (fread(data.buf, sizeof(char), data.len, f) != (usize)data.len)
    {
        perror("fread");
        return Result<Str, FileError>(Str(""), FILE_FAIL_READ);
    }

    if (fclose(f) != 0)
    {
        perror("fclose");
        return Result<Str, FileError>(data, FILE_FAIL_CLOSE);
    }

    return Result<Str, FileError>(data, FILE_SUCCESS);
}

Result<Str, FileError> File_Write(Arena* a, Str path, Str text)
{
    Arena temp      = *a;
    char* path_cstr = path.Cstr(&temp); // We dont need path after loading buf

    FILE* f = fopen(path_cstr, "wb");
    if (f == NULL)
    {
        perror("fopen");
        return Result<Str, FileError>(Str(""), FILE_FAIL_OPEN);
    }

    if (fwrite(text.buf, sizeof(char), text.len, f) != (usize)text.len)
    {
        perror("fwrite");
        return Result<Str, FileError>(Str(""), FILE_FAIL_WRITE);
    }

    if (fclose(f) != 0)
    {
        perror("fclose");
        return Result<Str, FileError>(text, FILE_FAIL_CLOSE);
    }

    return Result<Str, FileError>(text, FILE_SUCCESS);
}
