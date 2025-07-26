#include "doctest.h"
#include "slice.h"

int main()
{

    TEST_SUITE("Slice")
    {

        typedef struct Item {
            i32 a = 4;
            i32 b = 8;
        } Item;

        typedef Slice<i32> Si32;

        Arena perm = Arena(1024); // 1 KB

        TEST_CASE("Size: Constant across types")
        {
            CHECK(sizeof(Si32) == 24);        // 8(buf) + 8(len) + 8(cap)
            CHECK(sizeof(Slice<Item>) == 24); // independent of value type
        }

        TEST_CASE("Initialization: From arena")
        {
            Arena a = perm;

            Si32 s = Si32(&a, 2);
            CHECK(s.len == 0);
            CHECK(s.cap == 2);

            // CHECK(s[0] == 0); // Zero init, so cannot access 0th element
            //                      [E] Failed: i < len

            // Append elements
            s + 1;
            s + 2;

            // Check by value
            CHECK(*s[0] == 1);
            CHECK(*s[1] == 2);

            // s + 2; // Will print error and drop
            //           [E] Overflow: len + 1 (3) <= cap (2)
            //           Dropping item
        }

        TEST_CASE("Initialization: From arena -> using {...}")
        {
            Arena a = perm;
            Si32  s = {&a, 2};
            CHECK(s.len == 0);
            CHECK(s.cap == 2);
        }

        TEST_CASE("Initialization: From fields -> using {...}")
        {
            Arena a = perm;
            Si32  s = {&a, 2};
            s + 1;
            s + 2;
            Si32 x = {s.buf, s.len, s.cap};
            RANGE(i, x.len) { CHECK(*s[i] == *x[i]); }
        }

        TEST_CASE("Initialization: From const array -> using {...}")
        {
            Si32 x = {(i32[]){1, 2}, 2};
            CHECK(x.len == x.cap);
            CHECK(*x[0] == 1);
            CHECK(*x[1] == 2);
        }

        TEST_CASE("Destructor: Reclaim arena -> using Final(...)")
        {
            Arena a = perm;

            // Initialize to some maxlen
            Si32 x = {&a, 10};
            CHECK(a.Used() == 40);

            // Reclaim after append
            x + 1;
            x + 2;
            x = x.Final(&a);
            CHECK(a.Used() == 8);

            // Check that values are valid
            CHECK(*x[0] == 1);
            CHECK(*x[1] == 2);
        }

        TEST_CASE("operator+: Typical usage")
        {
            Arena a = perm;

            // Initialize a slice
            Si32 s = {&a, 10};

            // Append
            RANGE(i, s.cap) { s + i; }

            // Check values
            CHECK(s.len == s.cap);
            RANGE(i, s.len){CHECK(*s[i] == i)};
        }

        TEST_CASE("operator+: Append more than capacity")
        {
            Arena a = perm;

            Si32 s = {&a, 0};

            // s + 2; // Will print error and drop
            //           [E] Overflow: len + 1 (3) <= cap (2)
            //           Dropping item
            //           TODO: Any way of silencing the error?

            CHECK(s.len == s.cap);
        }

        TEST_CASE("operator[i]: By reference")
        {
            Arena a = perm;

            Si32 s = {&a, 3};          // Init
            RANGE(i, s.cap) { s + i; } // Append

            *s[0] = 10;         // Mutate
            CHECK(*s[0] == 10); // Mutated value
        }

        TEST_CASE("operator[i, j]: By reference")
        {
            Arena a = perm;

            // Initialize a slice
            Si32 s = {&a, 10};
            RANGE(i, s.cap) { s + i; } // Append

            // Take subslice by reference
            auto sub = s[5, 8];

            // Confirm value
            CHECK(*sub[0] == *s[5]);

            // Mutate value in sub and original
            *sub[1] = 53;
            CHECK(*sub[1] == 53); // Mutated value in subslice
            CHECK(*s[6] == 53);   // Mutated origial
        }

        TEST_CASE("operator[i, j, a]: By value (copy to arena)")
        {
            Arena a = perm;

            // Initialize a slice
            Si32 s = {&a, 10};
            RANGE(i, s.cap) { s + i; } // Append

            // Take subslice by value
            auto sub = s[5, 8, &a];

            // Confirm value
            CHECK(*sub[0] == *s[5]);

            // Mutate value in sub and original
            *sub[1] = 53;
            CHECK(*sub[1] == 53); // Mutated value in subslice
            CHECK(*s[6] == 6);    // Retains original value
        }

        TEST_CASE("operator==: Rock solid behaviour")
        {
            // NOTE: Had to keep track of lengths manually, can we use `size`?
            Si32 s1 = {(i32[]){1, 2}, 2, 2};
            Si32 s2 = {(i32[]){3, 4}, 2, 2};
            Si32 s3 = {(i32[]){3, 4}, 2, 2};
            Si32 s4 = {(i32[]){4, 3}, 2, 2};
            Si32 s5 = {(i32[]){3, 4, 5}, 3, 3};
            Si32 s6 = {(i32[]){3}, 1, 1};
            Si32 s7 = {(i32[]){}, 0, 0}; // Same as null pointer for first element (s8)
            Si32 s8 = {0, 0, 0};

            // Self Equality
            CHECK(s1 == s1);
            CHECK(s2 == s2);
            CHECK(s6 == s6);

            // Zero equality
            CHECK(s7 == Si32((i32[]){}, 0, 0));

            // Null equality
            CHECK(s8 == s8);

            // Null zero equality
            CHECK(s8 == s7);

            // Cross Equality
            CHECK(s2 == s3);

            // Subset inequality
            CHECK(s2 != s6);

            // Superset inequality
            CHECK(s2 != s5);

            // Zero inequality
            CHECK(s2 != s7);

            // Null inequality
            CHECK(s2 != s8);
        }
    }

    return 0;
}
