#include "table.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_SUITE("Table")
{

    TEST_CASE("Stuct size")
    {
        CHECK(sizeof(Table) == 16); // 8(nrows) + 8(ncols)
    }

}
