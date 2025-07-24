// S_ISBLK(m)
//     Test for a block special file.
// S_ISCHR(m)
//     Test for a character special file.
// S_ISDIR(m)
//     Test for a directory.
// S_ISFIFO(m)
//     Test for a pipe or FIFO special file.
// S_ISREG(m)
//     Test for a regular file.
// S_ISLNK(m)
//     Test for a symbolic link.
// S_ISSOCK(m)
//     Test for a socket.

#pragma once

#include <cstdio>
#include <cstring>
#include <ctime>
#include <sys/stat.h>

#include "log.h"
#include "slice.h"
#include "str.h"

// TODO: Use Str assuming null terminated, as validated by strlen
typedef struct Path {
    Str  path   = {};
    bool exists = false;

    struct stat info = {}; // Zero init (e.g. times for non-existent files)

    Strs parts = {};

    Path() = default;

    Path(Str path_)
    {
        path    = path_;
        int ret = stat(path.buf, &info);
        exists  = (ret == 0);
    }

    bool is_dir() { return S_ISDIR(info.st_mode); }
    bool is_file() { return S_ISREG(info.st_mode); }
    bool is_symlink() { return S_ISLNK(info.st_mode); }
    bool is_fifo() { return S_ISFIFO(info.st_mode); }
    bool is_socket() { return S_ISSOCK(info.st_mode); }
    bool is_block_device() { return S_ISBLK(info.st_mode); }
    bool is_char_device() { return S_ISCHR(info.st_mode); }

    char* time_stat() { return ctime(&info.st_ctime); }
    char* time_mod() { return ctime(&info.st_mtime); }
    char* time_access() { return ctime(&info.st_atime); }

    void Parse(Arena* a) { parts = Str(path).Split(a, "/", false); }

    // TODO: Edge cases -> multiple '/', starting '/', ending '/'
    Str name()
    {
        Fatal(parts.len == 0, "Parse(Arena* a) needs to be invoked");
        return parts.data[parts.len - 1];
    }

    // TODO: Needs arenaless join
    Str parent()
    {
        Fatal(parts.len == 0, "Parse(Arena* a) needs to be invoked");
        return parts.data[parts.len - 1];
    }

    Slice<Path> Glob(Arena* a);

    void Print(bool verbose = false)
    {
        printf(COLOR_BLUE_BOLD "%.*s\n" COLOR_RESET, pstr(path));

        if (verbose)
        {
            Fatal(parts.len == 0, "Parse(Arena* a) needs to be invoked");
            printf("  |      name: %.*s\n", pstr(name()));
            printf("  | parts.len: %ld\n", parts.len);

            printf("  |      stat: %s", time_stat());
            printf("  |       mod: %s", time_mod());
            printf("  |    access: %s", time_access());
        }
    }

} Path;
