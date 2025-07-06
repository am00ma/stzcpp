#include "buf.h"
#include <cstdio>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_SUITE("Buf")
{

    TEST_CASE("Stuct size")
    {
        // The struct itself is very light, with 3 64 bit integers
        CHECK(sizeof(Buf) == 24); // 8(buf) + 8(len) + 8(cap)
    }

    Arena perm = Arena(1024 * 4);

    TEST_CASE("Usage: Use full maxlen")
    {
        Arena a   = perm;
        Buf   buf = Buf(&a, 512);

        Str src = "hello hi, alles good?\n";

        // Some example parsing algo
        RANGE(i, src.len)
        {
            if (src.buf[i] == 'l')
            {
                Arena temp  = Arena();
                Str   found = Str(&a, 32, "Found l: %d\n", i);
                buf.Join(found); // Copies from end of arena to buf
            }
        }

        Str out      = Str(buf.buf, buf.len);
        Str expected = "Found l: 2\n"
                       "Found l: 3\n"
                       "Found l: 11\n"
                       "Found l: 12\n";
        CHECK(out == expected);
        CHECK(a.Used() == 558);
    }

    TEST_CASE("Usage: Use temp buffer to optimize mem usage")
    {
        Arena a    = perm;
        Buf   buf  = Buf(&a, 512); // Alloc buffer
        Arena temp = a;            // Space 'above' buffer

        // Some example parsing algo
        Str src = "hello hi, alles good?\n";

        RANGE(i, src.len)
        {
            if (src.buf[i] == 'l')
            {
                Str found = Str(&temp, 32, "Found l: %d\n", i);
                buf.Join(found); // Copies from end of arena to buf
            }
        }

        // Reclaims space from arena, if guaranteed that
        // buf initialization was last use of arena (apart from scratch)
        Str out = buf.Final(&a);

        Str expected = "Found l: 2\n"
                       "Found l: 3\n"
                       "Found l: 11\n"
                       "Found l: 12\n";
        CHECK(out == expected);
        CHECK(a.Used() == 46);
    }
}
