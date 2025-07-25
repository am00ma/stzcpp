#include "util/db.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"

TEST_SUITE("DB")
{

    TEST_CASE("Stuct size")
    {
        CHECK(sizeof(Db) == 16); // 16(Str)
    }
}
