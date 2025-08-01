#include "doctest.h"
#include "list.h"

int main()
{
    TEST_SUITE("List")
    {

        TEST_CASE("Size: struct")
        {
            // Independent of template type
            isize sz = 16;
            TEqual(sizeof(List<char>), sz, "%ld");
            TEqual(sizeof(List<u32>), sz, "%ld");
            TEqual(sizeof(List<u16>), sz, "%ld");
        }

        // Create arena on stack
        BufArena(perm, abuf, 4096);

        TEST_CASE("Template type: i32, char")
        {
            // Need to specify (i32[]) though
            List<i32> b = (i32[]){1, 2, 3};
            TEqual(b.len, (isize)3, "%ld");

            // Treats '\0' as valid char and part of len
            List<char> a = "hello";
            TEqual(a.len, (isize)6, "%ld");
        }

        TEST_CASE("Template type: custom structs")
        {
            struct s1 {
                i32 a;
                i16 b;
            } s1_ = {};

            // Need to specify (s1[]) though
            List<s1> b = (s1[]){{1, 2}, {3, 4}, {5, 6}};
            TEqual(b.len, (isize)3, "%ld");
        }

        TEST_CASE("Initialization: Zero-init")
        {
            List<char> x0 = {};
            TEqual(x0.buf, (void*)0, "%p");
            TEqual(x0.len, (isize)0, "%ld");

            List<i32> x1 = {};
            TEqual(x1.buf, (void*)0, "%p");
            TEqual(x1.len, (isize)0, "%ld");
        }

        TEST_CASE("Initialization: From fields")
        {
            List<char> x0 = "hello";
            List<char> x1 = {x0.buf, x0.len};
            TEqual(x1.buf, x0.buf, "%p");
            TEqual(x1.len, x0.len, "%ld");
        }

        TEST_CASE("Initialization: From literals")
        {
            List<char> x0 = "hello";
            TNotEqual(x0.buf, (void*)0, "%p");
            TEqual(x0.len, (isize)6, "%ld");

            List<i32> x1 = (i32[]){1, 2, 3};
            TEqual(x1.len, (isize)3, "%ld");
        }

        TEST_CASE("Initialization: From arena")
        {
            Arena temp = perm;

            List<i32> x = {&temp, 3,  NOZERO};
            RANGE(i, x.len) { *x[i] = i; };
            RANGE(i, x.len) { TEqual(*x[i], (i32)i, "%d"); };
        }

        TEST_CASE("Operator ==: Equality")
        {
            struct s1 {
                i32  a;
                i16  b;
                bool operator==(s1 s) { return ((a == s.a) && (b == s.b)); };
            } s1_ = {};

            List<s1> x0 = {};
            TCheck(x0 == x0); // Nulls are equal

            List<s1> x1 = (s1[]){{2, 4}};
            List<s1> x2 = (s1[]){{2, 4}};
            List<s1> x3 = (s1[]){{3, 4}};
            List<s1> x4 = (s1[]){{4, 2}};

            TCheck(x1 != x0);

            TCheck(&x1 != &x2);
            TCheck(x1 == x2);

            TCheck(x1 != x3);
            TCheck(x1 != x4);
        }

        TEST_CASE("Operator [i]: Indexing")
        {
            List<char> x0 = "hello";
            TEqual(*x0[0], 'h', "%c");
            TEqual(*x0[1], 'e', "%c");
            TEqual(*x0[2], 'l', "%c");
            TEqual(*x0[3], 'l', "%c");
            TEqual(*x0[4], 'o', "%c");
            TEqual(*x0[5], '\0', "%c");
            // TEqual(*x0[6], '\0', "%c"); // Will segfault

            // -0 == 0
            TEqual(*x0[-0], 'h', "%c");

            TEqual(*x0[-1], '\0', "%c");
            TEqual(*x0[-2], 'o', "%c");
            TEqual(*x0[-3], 'l', "%c");
            TEqual(*x0[-4], 'l', "%c");
            TEqual(*x0[-5], 'e', "%c");
            TEqual(*x0[-6], 'h', "%c");
            // TEqual(*x0[-7], '\0', "%c"); // Will segfault
        }

        TEST_CASE("Operator [i, j]: Sublist - By reference")
        {
            List<char> x0 = "hello";
            x0.len--; // Remove '\0' so negative indexing works

            List<char> x1 = "llo";
            x1.len--; // Remove '\0' so we can compare

            List<char> x2 = "ll";
            x2.len--; // Remove '\0' so we can compare

            List<char> y = {};

            y = x0[2, 5]; // llo
            TCheck(y == x1);

            y = x0[-3, 5]; // llo
            TCheck(y == x1);

            y = x0[-3, -1]; // ll
            TCheck(y == x2);

            y = x0[-1, -3]; // null
            TCheck(y.buf == x0[-1]);
            TCheck(y.len == 0);

            // buf points to correct pos, but len is 0
            y = x0[-1, -1];
            TCheck(y.buf == x0[-1]);
            TCheck(y.len == 0);

            // Borrow some memory
            Arena a = perm;

            // Needs mutable, not literal
            List<char> z = {&a, x0.len,  NOZERO};
            RANGE(i, x0.len) { *z[i] = *x0[i]; }
            y = z[2, 5];

            // Mutate
            *y[0] = 'g';
            *y[1] = 'g';

            // Check mutation
            List<char> x4 = "ggo";
            x4.len--;
            TCheck(y == x4);
        }

        TEST_CASE("Method: Copy (well-aligned)")
        {
            // Borrow some memory
            Arena a = perm;

            // TODO: Needs to be checked

            // Copy to arena
            List<char> x0 = "hello";
            List<char> y  = x0.Copy(&a);
            TCheck(y == x0);

            // Check usage (hello + \0)
            TEqual(a.Used(), (isize)6, "%ld");

            // Reset arena
            a = perm;

            // Copy to arena
            List<i64> z0 = (i64[]){1, 2, 3, 4, 5, 6};
            List<i64> y1 = z0.Copy(&a);
            TCheck(y1 == z0);

            // Check usage
            TEqual(a.Used(), sizeof(i64) * z0.len, "%ld");

            // Reset arena
            a = perm;

            // Copy to arena
            List<u32> z1 = (u32[]){1, 2, 3, 4, 5, 6};
            List<u32> y2 = z1.Copy(&a);
            TCheck(y2 == z1);

            // Check usage
            TEqual(a.Used(), sizeof(u32) * z1.len, "%ld");

            // Reset arena
            a = perm;

            // Copy to arena
            List<u32[3]> z2 = (u32[][3]){{1, 1, 1}, {2, 2, 2}};
            RANGE(i, 2) RANGE(j, 3) { TEqual((u32)i + 1, (*z2[i])[j], "%d"); }

            List<u32[3]> y3 = z2.Copy(&a);
            RANGE(i, 2) RANGE(j, 3)
            {
                TEqual((*y3[i])[j], (u32)i + 1, "%d");
                TEqual((*y3[i])[j], (*z2[i])[j], "%d");
            }

            // Check usage
            TEqual(a.Used(), sizeof(i32) * 3 * z2.len, "%ld");
        }

        TEST_CASE("Method: Copy (half-aligned)")
        {
            // Borrow some memory
            Arena a = perm;

            // Copy to arena
            List<i16> x0 = (i16[]){1, 2, 3, 4, 5, 6};
            List<i16> y0 = x0.Copy(&a);
            TCheck(y0 == x0);

            // Check usage
            TEqual(a.Used(), sizeof(i16) * x0.len, "%ld");

            // Reset arena
            a = perm;

            // Copy to arena
            List<i16[3]> z2 = (i16[][3]){{1, 1, 1}, {2, 2, 2}};
            RANGE(i, 2) RANGE(j, 3) { TEqual((i16)i + 1, (*z2[i])[j], "%d"); }

            List<i16[3]> y3 = z2.Copy(&a);
            RANGE(i, 2) RANGE(j, 3)
            {
                TEqual((*y3[i])[j], (i16)i + 1, "%d");
                TEqual((*y3[i])[j], (*z2[i])[j], "%d");
            }

            // Check usage
            TEqual(a.Used(), sizeof(i16) * 3 * z2.len, "%ld");
        }
    }
}
