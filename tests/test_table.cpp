#include "arena.h"
#include "table.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

struct RowType {
    isize a;
    isize b;

    Str Print(isize idx, Arena* arena)
    {
        Str ret = "";
        switch (idx)
        {
        case 0: ret = Str(arena, 64, "%ld", a); break;
        case 1: ret = Str(arena, 64, "%ld", b); break;
        default: break;
        }
        return ret;
    };
};

TEST_SUITE("Table")
{

    TEST_CASE("Stuct size")
    {

        CHECK(sizeof(Table<RowType>) == 8 + 8 + 24); // 16(Strs) + 24(Slice)
    }

    Arena a = Arena(1024 * 1024);

    TEST_CASE("Initialization")
    {
        Arena temp = a;

        Str  names[] = {"a", "b"};
        Strs cols    = Strs(names, 2);

        auto table = Table<RowType>(cols, 10, &temp);

        RANGE(i, table.rows.cap) { table.rows.Append({i, i + 10}); }

        table.Print();
    }
}
