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
        Arena a = perm;

        Str text = "hellohi";
        Str path = "/tmp/stz-file-tmpfile";

        FileResult res_w = File_Write(&a, path, text);
        TEqualInt(res_w.err, 0);
        TEqualLong(a.Used(), 0);

        FileResult res_r = File_Read(&a, path);
        TEqualInt(res_r.err, 0);
        TEqualStr(res_r.data, text);
        TEqualLong(a.Used(), text.len);
    }

    TEST_RESULTS();

    return 0;
}
