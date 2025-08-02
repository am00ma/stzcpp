#pragma once

#include <ctime>      // ctime
#include <glob.h>     // glob, globfree
#include <sys/stat.h> // stat
#include <unistd.h>   // unlink

#include "log.h"
#include "str.h"

typedef struct Path {

    Str  path   = {};
    bool exists = false;

    struct stat info = {}; // Zero init (e.g. times for non-existent files)

    Path() = default;

    Path(Str path_)
    {
        // Non-owning (underlying buf of Str is not in our control)
        path = path_;

        // Remove trailing slash
        if (path.buf[path.len - 1] == '/') { path.len--; }

        // Use proper null terminated string
        BufArena(temp, buf, 4096);
        char* cpath = path.Cstr(&temp);

        // Get stats (dir/file, size, etc)
        int ret = stat(cpath, &info);
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

    // TODO: Edge cases -> multiple '/', starting '/', ending '/'
    Str name()
    {
        isize len = 0;
        RANGE(i, path.len)
        {
            if (path.buf[i] == '/') { len = i; }
        }
        if (len == 0) { return path; }; // No '/' found => full path is name
        Str p = Str(&path.buf[len + 1], path.len - len - 1);
        return p;
    }

    // TODO: Edge cases -> multiple '/', starting '/', ending '/'
    Path parent()
    {
        isize len = -1;
        RANGE(i, path.len)
        {
            if (path.buf[i] == '/') { len = i; }
        }
        if (len == -1) { return {}; }
        return Path({path.buf, len});
    }

    void Print(bool verbose = false)
    {
        printf(COLOR_BOLD_BLUE "%.*s\n" COLOR_RESET, pstr(path));
        if (verbose)
        {
            printf("    name: %.*s\n", pstr(name()));
            printf("  parent: %.*s\n", pstr((parent().path.len ? parent().path : Str(""))));
        }
    }

} Path;

List<Path> Glob(List<Str> patterns, Arena* a);
