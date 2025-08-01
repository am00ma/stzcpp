#include "doctest.h"
#include "str.h"

int main()
{
    TEST_SUITE("Str")

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

    TEST_CASE("Buf: Initialization: from Str")
    {
        // Buf a = "hello"; // Not indended use, so disallowed

        Buf a = Str("hello"); // If necessary
        TEqualStr(a, Str("hello"));
        TEqualLong(a.len, 5);
        TEqualLong(a.cap, 5);
    }

    TEST_CASE("Buf: Initialization: from Str")
    {
        // Buf a = "hello"; // Not indended use, so disallowed

        Buf a = Str("hello"); // If necessary
        TEqualStr(a, Str("hello"));
        TEqualLong(a.len, 5);
        TEqualLong(a.cap, 5);

        // Buf b = a[1, 4]; // Not intended usage -> returns List<char>
        Str b = a[1, 4]; // Allowed
        TEqualStr(b, Str("ell"));
    }

    TEST_CASE("Buf: Copy")
    {
        char cbuf[16] = {};

        Buf b = {cbuf};
        TEqualLong(b.len, 0);
        TEqualLong(b.cap, 16);

        // Treats as Str
        b += "hello";
        TEqualLong(b.len, 5);
        TEqualLong(b.cap, 16);

        // Can request special treatment
        b  = {cbuf};
        b += List<char>("hello");
        TEqualLong(b.len, 6);
        TEqualLong(b.cap, 16);

        // Or push off to arena using Push
        Arena a = {1024};
        b       = {cbuf};
        TEqualLong(b.cap, 16);
        b.Push(&a, "hello hi how are you today");
        TEqualLong(b.len, 26);
        TEqualLong(b.cap, 32);

        // Or finish a set concat then copy to arena
        a = {1024};
        b = {cbuf}; // cap 16

        b          += "hello ";
        b          += "hi ";
        Buf b_copy  = Str(b.Copy(&a)); // Needed to cast List<char>
        TEqualLong(b_copy.len, 6 + 3);
        TEqualLong(b_copy.cap, 6 + 3);
        TEqualLong(a.Used(), 9);
        TEqualStr(b_copy, Str("hello hi "));

        b_copy.Push(&a, "hello hi how are you today"); // Have to cast to str :(
        TEqualLong(b.len, 9);
        TEqualLong(b.cap, 16);
        TEqualLong(b_copy.len, 26 + 6 + 3); // = 35
        TEqualLong(b_copy.cap, 36);         // 9 * 2 * 2 = 36
        TEqualLong(a.Used(), 36);           // As much as buffer
        TEqualStr(b_copy, Str("hello hi hello hi how are you today"));

        b_copy.Push(&a, "hello");
        TEqualLong(b_copy.len, 40); // 35 + 5 (expected)
        TEqualLong(b_copy.cap, 72); // 36 * 2 (expected)
        TEqualLong(a.Used(), 72);   // As much as buffer
        TEqualStr(b_copy, Str("hello hi hello hi how are you todayhello"));

        RANGE(i, 10) { b_copy.Push(&a, "hello"); }
        TEqualLong(b_copy.len, 90);  // 40 + 50    (expected)
        TEqualLong(b_copy.cap, 144); // 36 * 2 * 2 (expected)
        TEqualLong(a.Used(), 144);   // As much as buffer
    }

    TEST_CASE("Buf: Initialization: from static char[]") {}

    TEST_RESULTS();
}
