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
        path.Parse(&a);
        path.Print(true);

        // Current directory
        path = Path(".");
        CHECK(path.exists);
        path.Parse(&a);
        path.Print(true);

        // Trailing slash
        path = Path("./");
        CHECK(path.exists);
        path.Parse(&a);
        path.Print(true);
    }

    TEST_CASE("not exists")
    {
        Arena a    = perm;
        Path  path = {};

        // Empty string
        path = Path("");
        CHECK(!path.exists);
        path.Parse(&a);
        path.Print(true);

        // Random file
        path = Path("./hello");
        CHECK(!path.exists);
        path.Parse(&a);
        path.Print(true);

        // Does not recognize home directory yet
        path = Path("~");
        CHECK(!path.exists);
        path.Parse(&a);
        path.Print(true);
    }

    TEST_CASE("parts")
    {
        Arena a = perm;

        Path path = Path("/tmp/hello/hi/how.are.you");
        path.Parse(&a);

        path.Print(true);
    }
}
