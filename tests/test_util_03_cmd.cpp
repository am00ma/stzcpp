#include "doctest.h"
#include "util/cmd.h"

int main()
{

    TEST_SUITE("Arena")
    {

        TEST_CASE("Stuct size")
        {
            TEqualLong(sizeof(Cmd), (16 * 2) + 8); // stdout, stderr, status (i32)
        }

        Arena perm = Arena(4 * 1024); // 4 KB

        TEST_CASE("Normal command")
        {
            Arena a = perm;

            isize out_maxlen = 3 * 1024;
            isize err_maxlen = 1 * 1024;
            Str   shell      = "/bin/sh";

            auto result = Cmd_Exec(&a, "ls -lah /usr/local/bin", out_maxlen, err_maxlen, shell);
            TEqualInt(result.err, 0);
            TEqualLong(a.Used(), 2931);
        }
    }

    TEST_RESULTS();
}
