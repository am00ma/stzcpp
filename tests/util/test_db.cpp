#include "log.h"
#include "slice.h"
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

    TEST_CASE("Create Table")
    {
        Db db = Db("/tmp/tmp.db");
        db.Print();

        // Columns
        Slice<DbColumn> columns = Slice<DbColumn>(&db.mem, 4);
        columns.Append(DbColumn(0, "int", CELL_INTEGER, true, "nan", true));
        columns.Append(DbColumn(1, "text", CELL_TEXT));
        columns.Append(DbColumn(2, "float", CELL_REAL));
        columns.Append(DbColumn(3, "blob", CELL_BLOB));

        Str     ret = {};
        DbError err = {};

        ret = StmtCreateTable("newtable", columns, &db.mem);
        debug("%.*s\n", pstr(ret));

        err = db.ExecVoid(ret);
        CheckErr(err, "ExecVoid(SQL_CREATE_TABLE) failed");

        ret = StmtCreateTable("newtable2", columns, &db.mem);
        debug("%.*s\n", pstr(ret));

        err = db.ExecVoid(ret);
        CheckErr(err, "ExecVoid(SQL_CREATE_TABLE) failed");

        db.Print();
    }
}
