#include "arena.h"
#include "buf.h"

#include "doctest.h"

int main()
{

    TEST_SUITE("Buf")
    {

        TEST_CASE("Stuct size")
        {
            CHECK(sizeof(Buf) == 24); // 8(buf) + 8(len) + 8(cap)
        }

        Arena perm = Arena(1024 * 4);

        TEST_CASE("Initialization")
        {
            Arena a   = perm;
            Buf   buf = Buf(&a, a.cap);

            Buf tmp = {};

            // Append Str and return Str
            tmp = *(buf + "Hello");

            // Empty brackets returns full Str (till len)
            CHECK(tmp[] == Str("Hello"));

            // No gaps
            tmp = *(buf + "Hi");
            CHECK(tmp[] == Str("HelloHi"));

            // Confirm arena usage, return string
            CHECK(a.Used() == buf.cap);
            Str out = buf.Final(&a);
            CHECK(out == Str("HelloHi"));
            CHECK(a.Used() == out.len);
            CHECK(a.Used() == buf.len);
        }

        TEST_CASE("Usage: Use full maxlen")
        {
            Arena a   = perm;
            Buf   buf = Buf(&a, 512);

            Str src = "hello hi, alles good?\n";

            // Find 'l' in src
            RANGE(i, src.len)
            {
                if (src.buf[i] == 'l')
                {
                    BufArena(temp, cbuf, 64);                       // To avoid this, check below [1]
                    Str found = Str(&temp, 32, "Found l: %d\n", i); // String in temp arena
                    buf + found;                                    // Copies from temp arena to buf
                }
            }

            Str out      = Str(buf.buf, buf.len);
            Str expected = "Found l: 2\n"
                           "Found l: 3\n"
                           "Found l: 11\n"
                           "Found l: 12\n";
            CHECK(out == expected);
            CHECK(a.Used() == buf.cap);
        }

        TEST_CASE("Usage: Use temp buffer to optimize mem usage")
        {
            Arena a    = perm;
            Buf   buf  = Buf(&a, 512); // Alloc buffer
            Arena temp = a;            // Space 'above' buffer to avoid BufArena [1]

            // Find 'l' in src
            Str src = "hello hi, alles good?\n";

            RANGE(i, src.len)
            {
                if (src.buf[i] == 'l')
                {
                    Str found = Str(&temp, 32, "Found l: %d\n", i); // String in temp arena
                    buf + found;                                    // Copies from temp arena to buf
                }
            }

            // Reclaims space from arena, if guaranteed that
            // buf initialization was last use of arena (apart from scratch)
            // Note that while temp is modified, a was not affected after buf init
            Str out = buf.Final(&a);

            Str expected = "Found l: 2\n"
                           "Found l: 3\n"
                           "Found l: 11\n"
                           "Found l: 12\n";
            CHECK(out == expected);
            CHECK(a.Used() == 46);
        }
    }

    return 0;
}
