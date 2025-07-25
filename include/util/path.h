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
#include <ctime>      // ctime
#include <glob.h>     // glob, globfree
#include <sys/stat.h> // stat
#include <unistd.h>   // execvp

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

    Slice<Path> Glob(Strs patterns, Arena* a)
    {
        int         err  = 0;
        Slice<Path> ret  = {};
        Arena       temp = *a;

        if (!is_dir()) { return ret; }
        if (patterns.len == 0) { return ret; }

        glob_t buf;
        buf.gl_offs = 1;

        // typedef struct {
        //     size_t   gl_pathc;    /* Count of paths matched so far  */
        //     char   **gl_pathv;    /* List of matched pathnames.  */
        //     size_t   gl_offs;     /* Slots to reserve in gl_pathv.  */
        // } glob_t;

        printf("pattern: %s\n", patterns.data[0].Cstr(&temp));
        err = glob(patterns.data[0].Cstr(&temp), GLOB_DOOFFS, NULL, &buf);
        Fatal(err, "Failed glob: %d (GLOB_NOSPACE: %d, GLOB_NOMATCH: %d)", err, GLOB_NOSPACE, GLOB_NOMATCH);

        RANGE(i, 1, patterns.len)
        {
            printf("pattern: %s\n", patterns.data[i].Cstr(&temp));
            err = glob(patterns.data[i].Cstr(&temp), GLOB_DOOFFS | GLOB_APPEND, NULL, &buf);
            Fatal(err, "Failed glob: %d (GLOB_NOSPACE: %d, GLOB_NOMATCH: %d)", err, GLOB_NOSPACE, GLOB_NOMATCH);
        }

        printf("Found %ld matches\n", buf.gl_pathc);
        RANGE(i, buf.gl_offs, buf.gl_pathc + buf.gl_offs)
        {
            printf("%zu: [%s]\n", i, (buf.gl_pathv[i] == NULL ? "<null>" : buf.gl_pathv[i]));
        }

        globfree(&buf);

        return ret;
    };

    void Print(bool verbose = false)
    {
        printf(COLOR_BLUE_BOLD "%.*s\n" COLOR_RESET, pstr(path));

        if (verbose)
        {
            Fatal(parts.len == 0, "Parse(Arena* a) needs to be invoked");
            printf("  |      name: %.*s\n", pstr(name()));
            printf("  | parts.len: %ld\n", parts.len);

            // printf("  |      stat: %s", time_stat());
            // printf("  |       mod: %s", time_mod());
            // printf("  |    access: %s", time_access());
        }
    }

} Path;
