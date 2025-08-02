#include "util/path.h"

// TODO: Keep glob buffer and dont copy?
List<Path> Glob(List<Str> patterns, Arena* a)
{
    int   err  = 0;
    Arena temp = *a;

    if (patterns.len == 0) { return {}; }

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

    Slice<Path> ret = {a, (isize)buf.gl_pathc};
    RANGE(i, buf.gl_offs, buf.gl_pathc + buf.gl_offs)
    {
        // Store in our arena, null terminated
        if (buf.gl_pathv[i]) { ret += Str(buf.gl_pathv[i], strlen(buf.gl_pathv[i])).Copy(a, true); }
    }

    globfree(&buf);

    return ret;
};
