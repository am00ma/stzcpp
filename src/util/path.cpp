#include "util/path.h"

int FileDelete(Str path)
{
    // Use proper null terminated string
    BufArena(temp, buf, 4096);
    char* cpath = path.Cstr(&temp);

    // Delete
    int err = unlink(cpath);
    return err;
}
