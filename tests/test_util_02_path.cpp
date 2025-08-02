#include "doctest.h"
#include "util/path.h"

int main()
{
    TEST_SUITE("util: path");

    BufArena(perm, abuf, 16 * 1024); // 16KB

    TEST_CASE("Fields: exists")
    {
        Path p = Str("./hello");
        TEqualInt(p.exists, 0);

        p = Str(__FILE__);
        TEqualInt(p.exists, 1);
    }

    TEST_CASE("Method Glob: current dir")
    {
        Arena a = perm;

        Str local_paths[] = {
            "/usr/local/bin", "/usr/local/cmake",   "/usr/local/etc",   "/usr/local/games",
            "/usr/local/go",  "/usr/local/include", "/usr/local/lib",   "/usr/local/libexec",
            "/usr/local/man", "/usr/local/sbin",    "/usr/local/share", "/usr/local/src",
        };
        isize num = sizeof(local_paths) / sizeof(Str);

        auto paths = Glob((Str[]){"/home/*"}, &a);
        TEqualLong(paths.len, 1);

        paths = Glob((Str[]){"/usr/local/*"}, &a);
        TEqualLong(paths.len, num);
        RANGE(i, paths.len) { TEqualStr(paths[i]->path, local_paths[i]); }
    }

    TEST_CASE("Method Delete: current dir")
    {
    }

    TEST_RESULTS();
}
