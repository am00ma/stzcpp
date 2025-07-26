#include "util/cmd.h"

#include "../doctest.h"

int main()
{

    TEST_SUITE("Arena")
    {

        TEST_CASE("Stuct size")
        {
            CHECK(sizeof(Cmd) == (16 * 2) + 8); // stdout, stderr, status (i32)
        }

        Arena a = Arena(32 * 1024 * 1024); // 32MB

        TEST_CASE("Normal command")
        {
            Arena temp = a;

            // Needs min of 32MB??
            auto result = Cmd_Exec(&temp, "ls -lah ./");
            CHECK(result.err == 0);
        }
    }

    return 0;
}
