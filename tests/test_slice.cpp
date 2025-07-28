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

        TEST_CASE("Initialization: From List")
        {
            Slice<char> x0 = List<char>("hello");
            Slice<char> x1 = {x0.buf, x0.len};
            TEqual(x1.buf, x0.buf, "%p");
            TEqual(x1.len, x0.len, "%ld");
            TEqual(x1.cap, x0.cap, "%ld");
        }

        TEST_CASE("Initialization: From arena")
        {
            Arena temp = perm;

            Slice<i32> x = {3, &temp, NOZERO};

            x.len = x.cap; // NOTE: Special case, not appending
            RANGE(i, x.len) { *x[i] = i; };
            RANGE(i, x.len) { TEqual(*x[i], (i32)i, "%d"); };
        }

        TEST_CASE("Operator +: Append items")
        {
            Arena temp = perm;

            Slice<i32> x = {3, &temp, NOZERO};
            List<i32>  y = {};
            RANGE(i, x.len) { y = x + i; };
            RANGE(i, x.len) { TEqual(*x[i], (i32)i, "%d"); };
        }

        TEST_CASE("Operator +: Extend items")
        {
            Arena temp = perm;

            Slice<i32> x = {3, &temp, NOZERO};
            List<i32>  y = (i32[]){1, 2, 3};
            x + y;
            TCheck(x == y);

            Slice<i32> x0 = {4, &temp, NOZERO};
            List<i32>  y1 = (i32[]){1, 2};
            List<i32>  y2 = (i32[]){3, 4};
            List<i32>  y3 = (i32[]){1, 2, 3, 4};
            x0 + y1;
            x0 + y2;
            TCheck(x0 == y3);
        }
    }
}
