#include "doctest.h"
#include "slice.h"
#include <cstdio>

int main()
{
    TEST_SUITE("Slice")
    {

        TEST_CASE("Size: struct")
        {
            // Independent of template type
            isize sz = 24;
            TEqual(sizeof(Slice<char>), sz, "%ld");
            TEqual(sizeof(Slice<u32>), sz, "%ld");
            TEqual(sizeof(Slice<u16>), sz, "%ld");
        }

        // Create arena on stack
        BufArena(perm, abuf, 4096);

        TEST_CASE("Template type: i32, char")
        {
            // Need to specify (List<i32>) though
            // Slice is not meant for literals
            Slice<i32> b = List<i32>({1, 2, 3});
            TEqual(b.len, (isize)3, "%ld");
            TEqual(*b[1], (i32)2, "%d");

            // Same behaviour as List while initializing
            // Treats '\0' as valid char and part of len
            Slice<char> a = List<char>("hello");
            TEqual(a.len, (isize)6, "%ld");
            TEqual(*a[1], 'e', "%c");
        }

        TEST_CASE("Template type: custom structs")
        {
            struct s1 {
                i32 a;
                i16 b;
            } s1_ = {};

            // Need to specify (List<s1>) though
            // Slice is not meant for literals
            Slice<s1> b = List<s1>({{1, 2}, {3, 4}, {5, 6}});
            TEqual(b.len, (isize)3, "%ld");
            TEqual(b.cap, (isize)3, "%ld");
            TEqual((*b[0]).a, (i32)1, "%d");
            TEqual((*b[0]).b, (i16)2, "%d");
            TEqual((*b[1]).a, (i32)3, "%d");
            TEqual((*b[1]).b, (i16)4, "%d");
            TEqual((*b[2]).a, (i32)5, "%d");
            TEqual((*b[2]).b, (i16)6, "%d");
        }

        TEST_CASE("Initialization: Zero-init")
        {
            Slice<char> x0 = {};
            TEqual(x0.buf, (void*)0, "%p");
            TEqual(x0.len, (isize)0, "%ld");
            TEqual(x0.cap, (isize)0, "%ld");

            Slice<i32> x1 = {};
            TEqual(x1.buf, (void*)0, "%p");
            TEqual(x1.len, (isize)0, "%ld");
            TEqual(x1.cap, (isize)0, "%ld");
        }

        //     TEST_CASE("Initialization: From fields")
        //     {
        //         Slice<char> x0 = "hello";
        //         Slice<char> x1 = {x0.buf, x0.len};
        //         TEqual(x1.buf, x0.buf, "%p");
        //         TEqual(x1.len, x0.len, "%ld");
        //     }

        //     TEST_CASE("Initialization: From literals")
        //     {
        //         Slice<char> x0 = "hello";
        //         TNotEqual(x0.buf, (void*)0, "%p");
        //         TEqual(x0.len, (isize)6, "%ld");
        //
        //         Slice<i32> x1 = (i32[]){1, 2, 3};
        //         TEqual(x1.len, (isize)3, "%ld");
        //     }

        TEST_CASE("Initialization: From arena")
        {
            Arena temp = perm;

            Slice<i32> x = {3, &temp, NOZERO};

            x.len = x.cap; // NOTE: Special case, not appending
            RANGE(i, x.len) { *x[i] = i; };
            RANGE(i, x.len) { TEqual(*x[i], (i32)i, "%d"); };
        }

        TEST_CASE("Operator ==: Equality with Slice / List")
        {
            struct s1 {
                i32  a;
                i16  b;
                bool operator==(s1 s) { return ((a == s.a) && (b == s.b)); };
            } s1_ = {};

            Slice<s1> x0 = {};
            TCheck(x0 == x0); // Nulls are equal

            Slice<s1> x1 = List<s1>({{2, 4}});
            Slice<s1> x2 = List<s1>({{2, 4}});
            Slice<s1> x3 = List<s1>({{3, 4}});
            Slice<s1> x4 = List<s1>({{4, 2}});

            // type(x1) == Slice | List, so == for List gives us == for Slice
            TCheck(x1 != x0);

            TCheck(&x1 != &x2);
            TCheck(x1 == x2);

            TCheck(x1 != x3);
            TCheck(x1 != x4);
        }

        TEST_CASE("Operator [i]: Indexing")
        {
            Slice<char> x0 = List<char>("hello");
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
            Slice<char> x0 = List<char>("hello");
            x0.len--; // Remove '\0' so negative indexing works

            Slice<char> x1 = List<char>("llo");
            x1.len--; // Remove '\0' so we can compare

            Slice<char> x2 = List<char>("ll");
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
            Slice<char> z = {x0.len, &a, NOZERO};

            z.len = z.cap; // NOTE: Special case, not appending
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

        TEST_CASE("Operator [i, j, a]: Sublist - By copy")
        {
            // Borrow some memory
            Arena a = perm;

            Slice<char> x0 = List<char>("hello");
            TEqual(x0.len, (isize)6, "%ld");
            x0.len--; // Remove '\0' so negative indexing works
            TEqual(x0.len, (isize)5, "%ld");

            Slice<char> x1 = List<char>("llo");
            x1.len--; // Remove '\0' so we can compare

            Slice<char> x2 = List<char>("ll");
            x2.len--; // Remove '\0' so we can compare

            List<char> y = {};

            y = x0[2, 5, &a]; // llo
            TCheck(y == x1);

            y = x0[-3, 5, &a]; // llo
            TCheck(y == x1);

            y = x0[-3, -1, &a]; // ll
            TCheck(y == x2);
        }

        TEST_CASE("Method: Copy (well-aligned)")
        {
            // Borrow some memory
            Arena a = perm;

            // TODO: Needs to be checked

            // Copy to arena
            Slice<char> x0 = List<char>("hello");
            List<char>  y  = x0.Copy(&a);
            TCheck(y == x0);

            // Check usage (hello + \0)
            TEqual(a.Used(), (isize)6, "%ld");

            // Reset arena
            a = perm;

            // Copy to arena
            Slice<i64> z0 = List<i64>({1, 2, 3, 4, 5, 6});
            List<i64>  y1 = z0.Copy(&a);
            TCheck(y1 == z0);

            // Check usage
            TEqual(a.Used(), sizeof(i64) * z0.len, "%ld");

            // Reset arena
            a = perm;

            // Copy to arena
            Slice<u32> z1 = List<u32>({1, 2, 3, 4, 5, 6});
            List<u32>  y2 = z1.Copy(&a);
            TCheck(y2 == z1);

            // Check usage
            TEqual(a.Used(), sizeof(u32) * z1.len, "%ld");

            // Reset arena
            a = perm;

            // Copy to arena
            Slice<u32[3]> z2 = List<u32[3]>({{1, 1, 1}, {2, 2, 2}});
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
            Slice<i16> x0 = List<i16>({1, 2, 3, 4, 5, 6});
            List<i16>  y0 = x0.Copy(&a);
            TCheck(y0 == x0);

            // Check usage
            TEqual(a.Used(), sizeof(i16) * x0.len, "%ld");

            // Reset arena
            a = perm;

            // Copy to arena
            Slice<i16[3]> z2 = List<i16[3]>({{1, 1, 1}, {2, 2, 2}});
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
