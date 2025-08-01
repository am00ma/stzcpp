#include "doctest.h"
#include "str.h"

int main()
{
    TEST_SUITE("Str")
    {

        // Dont even need malloc
        BufArena(perm, abuf, 1024);

        TEST_CASE("Size: struct")
        {
            isize sz = 16; // 8(buf) + 8(len)
            TEqual(sizeof(Str), sz, "%ld");
        }

        TEST_CASE("Initialization: from literals")
        {
            Str a = "hello";
            TNotEqual(a.buf, (char*)0, "%p"); // Valid
            TEqual(a.len, (isize)5, "%ld");   // '\0' not counted

            Str b = "";
            TNotEqual(b.buf, (char*)0, "%p");
            TEqual(b.len, (isize)0, "%ld");

            Str c = {};
            TEqual(c.buf, (char*)0, "%p");
            TEqual(c.len, (isize)0, "%ld");
        }

        TEST_CASE("Operator ==: with literals")
        {
            Arena a = perm;

            TEqualStr(Str("hello"), Str("hello"));
            TNotEqualStr(Str("hello"), Str("hi"));

            Str a1 = "hello";
            Str a2 = "hello hi";
            Str a3 = a1.Copy(&a);

            TEqualStr(a1, a1);
            TEqualStr(a2, a2);
            TEqualStr(a1, a3);
            TNotEqual(a1.buf, a3.buf, "%p");
        }

        TEST_CASE("Operator []: with literals")
        {
            Str a = "hello hi how are you";
            TEqual(a.len, (isize)20, "%ld");

            // Needs paranthesis, `Str(...)`
            TEqualStr((a[0, 5]), Str("hello"));
            TEqualStr((a[6, 8]), Str("hi"));
            TEqualStr((a[9, 12]), Str("how"));
            TEqualStr((a[13, 16]), Str("are"));
            TEqualStr((a[17, 20]), Str("you"));

            // Other way of init for check, `(Str) ...`
            Str b = a[0, 5];
            TEqualStr(b, (Str) "hello");
        }
    }

    TEST_RESULTS();
}
