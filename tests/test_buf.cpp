#include "buf.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_SUITE("Buf")
{

    TEST_CASE("Stuct size")
    {
        CHECK(sizeof(Buf) == 24); // 8(buf) + 8(len) + 8(cap)
    }

    Arena a = Arena(1024 * 4);

    TEST_CASE("Usage: Use full maxlen")
    {
        Arena a   = a;
        Buf   buf = Buf(&a, 512);

        Str src = "hello hi, alles good?\n";

        // Find 'l' in src
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
        Arena a    = a;
        Buf   buf  = Buf(&a, 512); // Alloc buffer
        Arena temp = a;            // Space 'above' buffer

        // Find 'l' in src
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
