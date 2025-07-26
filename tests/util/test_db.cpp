#include "log.h"
#include "slice.h"
#include "util/db.h"
#include "util/path.h"
#include "util/templates/sql/create.h"

#include "../doctest.h"

int main()
{
    TEST_SUITE("Db")
    {

        TEST_CASE("Stuct size")
        {
            CHECK(sizeof(Db) == 56); // 16(Str) + 8(bool) + 24(Arena) + 8(sqlite3*)
        }

        TEST_CASE("Initialization")
        {
            // Remove file if it exists
            Path path = Path("/tmp/tmp.db");
            if (path.exists) { CheckErr(FileDelete(path.path), "Could not Delete"); }

            // Refresh path, check that it deleted
            path = Path(path.path);
            CHECK(!path.exists);

            // Actually open, in SQL READ WRITE mode
            Db db = Db(path.path);
            CHECK(db.valid);

            // Create COMPANY table
            DbError err = db.ExecVoid(SQL_CREATE_TABLE);
            CheckErr(err, "ExecVoid(SQL_CREATE_TABLE) failed");

            // Remove test db
            path = Path(path.path);
            if (path.exists) { CheckErr(FileDelete(path.path), "Could not Delete"); }
        }

        TEST_CASE("Create Table")
        {
            // Remove test db
            Path path = Path("/tmp/tmp.db");
            if (path.exists) { CheckErr(FileDelete(path.path), "Could not Delete"); }

            Db db = Db(path.path, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 128 * 1024 * 1024);
            CHECK(db.valid);

            // Columns -> Need escaping for spaces
            Slice<DbColumn> columns = Slice<DbColumn>(&db.mem, 4);
            columns + DbColumn(0, "int", CELL_INTEGER, true, "nan", true);
            columns + DbColumn(1, "text", CELL_TEXT);
            columns + DbColumn(2, "float", CELL_REAL);
            columns + DbColumn(3, "blob", CELL_BLOB);

            Str     ret = {};
            DbError err = {};

            ret = StmtCreateTable("newtable", columns, &db.mem);
            err = db.ExecVoid(ret);
            CHECK(err == 0);

            ret = StmtCreateTable("newtable", columns, &db.mem);
            err = db.ExecVoid(ret);
            CHECK(err == 1);

            db.Print();

            // Remove test db
            path = Path(path.path);
            if (path.exists) { CheckErr(FileDelete(path.path), "Could not Delete"); }
        }
    }
}
