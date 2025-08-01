#include "arena.h"
#include "doctest.h"
#include "util/file.h"

int main()
{
    TEST_SUITE("util: file");

    BufArena(perm, abuf, 16 * 1024); // 16KB

    TEST_CASE("File not found")
    {
        Arena a   = perm;
        auto  res = File_Read(&a, "./hello");
        TEqualInt(res.err, FILE_FAIL_OPEN);
        TEqualLong(a.Used(), 0);
    }

    TEST_CASE("File found")
    {
        Arena a   = perm;
        auto  res = File_Read(&a, __FILE__);
        TEqualInt(res.err, 0);
        TEqualLong(a.Used(), 563);
    }

    TEST_RESULTS();

    return 0;
}
