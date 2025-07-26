#include "arena.h"
#include "table.h"

#include "doctest.h"

struct RowType {
    isize a;
    isize b;

    Str Print(isize idx, Arena* arena) { return Str(arena, 128, "%5ld, %5ld", a, b); };
};

TEST_SUITE("Table")
{

    TEST_CASE("Stuct size")
    {
        CHECK(sizeof(Table<RowType>) == 16 * 2 + 24); // 16*2(Strs) + 24(Slice) = 32+24 = 56
    }

    Arena a = Arena(1024 * 1024);

    TEST_CASE("Initialization")
    {
        Arena temp = a;

        Strs cols  = Strs((Str[]){"a", "b"}, 2);
        Strs types = Strs((Str[]){"isize", "isize"}, 2);

        auto table = Table<RowType>(cols, types, 10, &temp);

        RANGE(i, table.rows.cap + 2) { table.rows.Append({i, i + 10}); }

        table.Print();
    }
}
