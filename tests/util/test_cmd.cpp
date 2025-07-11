#include "util/cmd.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"

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

// int main()
// {
//     // ------------------------------------
//     constexpr i32 MB = 1024 * 1024;
//
//     Arena perm = Arena(18 * MB); // 16 for stdout, 2 for stderr
//     Arena temp = {0};
//
//     Str command = Str();
//
//     Result<Cmd, CmdError> result = {};
//
//     // ------------------------------------
//     title("\nls -lah ./");
//
//     temp    = perm;
//     command = "ls -lah ./";
//
//     result = Cmd_Exec(&temp, command);
//     if (!result.err) printf("Status: %d\n", result.data.status);
//
//     printf("stdout:\n%.*s\n", pstr(result.data.stdout));
//     printf("stderr:\n%.*s\n", pstr(result.data.stderr));
//
//     temp.Print("After `ls -lah ./` > temp:\n");
//     perm.Print("After `ls -lah ./` > perm:\n");
//
//     // ------------------------------------
//     title("\nls -lah ./abcdef");
//
//     temp    = perm;
//     command = Str("ls -lah ./abcdef");
//
//     result = Cmd_Exec(&temp, command);
//     printf("Status: %d\n", result.data.status);
//
//     printf("stdout:\n%.*s\n", pstr(result.data.stdout));
//     printf("stderr:\n%.*s\n", pstr(result.data.stderr));
//
//     temp.Print("After `ls -lah ./abcdef` > temp:\n");
//     perm.Print("After `ls -lah ./abcdef` > perm:\n");
//
//     // ------------------------------------
//     title("\nbash: ls -lah ./");
//
//     temp    = perm;
//     command = Str("ls -lah ./");
//
//     result = Cmd_Exec(&temp, command, 16 * MB, 2 * MB, "/usr/bin/bash");
//     printf("Status: %d\n", result.data.status);
//
//     printf("stdout:\n%.*s\n", pstr(result.data.stdout));
//     printf("stderr:\n%.*s\n", pstr(result.data.stderr));
//
//     temp.Print("After `ls -lah ./abcdef` > temp:\n");
//     perm.Print("After `ls -lah ./abcdef` > perm:\n");
//
//     return 0;
// }
