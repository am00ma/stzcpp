#include "arena.h"
#include "util/path.h"
#include <cstdio>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"

TEST_SUITE("Path")
{

    TEST_CASE("Stuct size")
    {
        CHECK(sizeof(Path) == 184); // Maybe bools can be condensed
    }

    Arena perm = Arena(1024 * 1024);

    TEST_CASE("exists")
    {
        Arena a    = perm;
        Path  path = {};

        // Current file
        path = Path(__FILE__);
        CHECK(path.exists);

        // Current directory
        path = Path(".");
        CHECK(path.exists);

        // Trailing slash
        path = Path("./");
        CHECK(path.exists);
    }

    TEST_CASE("not exists")
    {
        Arena a    = perm;
        Path  path = {};

        // Empty string
        path = Path("");
        CHECK(!path.exists);

        // Random file
        path = Path("./hello");
        CHECK(!path.exists);

        // Does not recognize home directory yet
        path = Path("~");
        CHECK(!path.exists);
    }

    TEST_CASE("parts")
    {
        Arena a    = perm;
        Path  path = Path("/tmp/hello/hi/how.are.you");
        path.Parse(&a);
        CHECK(path.parts.len == 5);
    }

    TEST_CASE("glob")
    {
        Arena a    = perm;
        Path  path = Path("./");

        Str  list[]   = {"*.md", "tests/**/*path.cpp", "**/range.h"};
        Strs patterns = Strs(list, 3);

        Slice<Path> paths = path.Glob(patterns, &a);

        CHECK(patterns.len == 3);
        CHECK(paths.len == 4);
        CHECK(paths[0]->path == Str("BUILDING.md"));
        CHECK(paths[1]->path == Str("README.md"));
        CHECK(paths[2]->path == Str("tests/util/test_path.cpp"));
        CHECK(paths[3]->path == Str("include/range.h"));
    }
}
