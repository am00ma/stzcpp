#pragma once

#include <ctime>      // ctime
#include <glob.h>     // glob, globfree
#include <sys/stat.h> // stat
#include <unistd.h>   // unlink

#include "arena.h"
#include "log.h"
#include "slice.h"
#include "str.h"

int FileDelete(Str path);

// TODO: Use Str assuming null terminated, as validated by strlen
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
        char* cpath = path_.Cstr(&temp);

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
        isize len = 0;
        RANGE(i, path.len)
        {
            if (path.buf[i] == '/') { len = i; }
        }
        Str p = Str(path.buf, len);
        return Path(p);
    }

    // TODO: Keep glob buffer and dont copy?
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

        err = glob((*patterns[0]).Cstr(&temp), GLOB_DOOFFS, NULL, &buf);
        if ((err != 0) && (err != GLOB_NOMATCH)) { Fatal(err, "Failed glob: %d", err); }

        RANGE(i, 1, patterns.len)
        {
            err = glob((*patterns[0]).Cstr(&temp), GLOB_DOOFFS | GLOB_APPEND, NULL, &buf);
            if ((err != 0) && (err != GLOB_NOMATCH)) { Fatal(err, "Failed glob: %d", err); }
        }

        ret = Slice<Path>(a, buf.gl_pathc);
        RANGE(i, buf.gl_offs, buf.gl_pathc + buf.gl_offs)
        {
            if (buf.gl_pathv[i])
            {
                Str f = Str(buf.gl_pathv[i], strlen(buf.gl_pathv[i])).Copy(a, true); // Copy to our arena
                ret.Append(Path(f));
            }
        }

        globfree(&buf);

        return ret;
    };

    void Print(bool verbose = false)
    {
        printf(COLOR_BOLD_BLUE "%.*s\n" COLOR_RESET, pstr(path));
        if (verbose)
        {
            printf("    name: %.*s\n", pstr(name()));
            printf("  parent: %.*s\n", pstr(parent().path));
        }
    }

} Path;
