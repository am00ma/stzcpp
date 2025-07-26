#include "log.h"
#include "util/db.h"
#include "util/templates/sql/create.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"

TEST_SUITE("Db")
{

    TEST_CASE("Stuct size")
    {
        CHECK(sizeof(Db) == 56); // 16(Str) + 8(bool) + 24(Arena) + 8(sqlite3*)
    }

    TEST_CASE("Initialization")
    {
        Db db = Db("/tmp/tmp.db");
        db.Print();

        DbError err = db.ExecVoid(SQL_CREATE_TABLE);
        CheckErr(err, "ExecVoid(SQL_CREATE_TABLE) failed");
    }
}
