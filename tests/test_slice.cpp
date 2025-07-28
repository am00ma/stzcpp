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
            TCheck(sizeof(Si32) == 24);        // 8(buf) + 8(len) + 8(cap)
            TCheck(sizeof(Slice<Item>) == 24); // independent of value type
        }

        TEST_CASE("Initialization: From arena")
        {
            Arena a = perm;

            Si32 s = Si32(&a, 2);
            TCheck(s.len == 0);
            TCheck(s.cap == 2);

            // TCheck(s[0] == 0); // Zero init, so cannot access 0th element
            //                      [E] Failed: i < len

            // Append elements
            s + 1;
            s + 2;

            // Check by value
            TCheck(*s[0] == 1);
            TCheck(*s[1] == 2);

            // s + 2; // Will print error and drop
            //           [E] Overflow: len + 1 (3) <= cap (2)
            //           Dropping item
        }

        TEST_CASE("Initialization: From arena -> using {...}")
        {
            Arena a = perm;
            Si32  s = {&a, 2};
            TCheck(s.len == 0);
            TCheck(s.cap == 2);
        }

        TEST_CASE("Initialization: From fields -> using {...}")
        {
            Arena a = perm;
            Si32  s = {&a, 2};
            s + 1;
            s + 2;
            Si32 x = {s.buf, s.len, s.cap};
            RANGE(i, x.len) { TCheck(*s[i] == *x[i]); }
        }

        TEST_CASE("Initialization: From const array -> using {...}")
        {
            Si32 x = {(i32[]){1, 2}, 2};
            TCheck(x.len == x.cap);
            TCheck(*x[0] == 1);
            TCheck(*x[1] == 2);
        }

        TEST_CASE("Initialization: From const array -> using {...}")
        {
            Si32 x = (const i32[]){1, 2, 3}; // Needs (const T[]) casting
            TCheck(*x[0] == 1);
            TCheck(*x[1] == 2);
            TCheck(*x[2] == 3);

            *x[1] = 4;          // Mutate
            TCheck(*x[1] == 4); // Check mutation
        }

        TEST_CASE("Destructor: Reclaim arena -> using Final(...)")
        {
            Arena a = perm;

            // Initialize to some maxlen
            Si32 x = {&a, 10};
            TCheck(a.Used() == 40);

            // Reclaim after append
            x + 1;
            x + 2;
            x = x.Final(&a);
            TCheck(a.Used() == 8);

            // Check that values are valid
            TCheck(*x[0] == 1);
            TCheck(*x[1] == 2);
        }

        TEST_CASE("operator+: Typical usage")
        {
            Arena a = perm;

            // Initialize a slice
            Si32 s = {&a, 10};
            Equal(a.Used(), (isize)40, "%ld");

            // Append, not all the way to cap
            RANGE(i, s.cap - 3) { s + i; }

            // Check values
            TCheck(s.len == s.cap - 3);
            RANGE(i, s.len){TCheck(*s[i] == i)};
            Equal(a.Used(), (isize)40, "%ld"); // Still using cap space

            // Reclaim space
            s.Final(&a);
            Equal(s.len, s.cap, "%ld");
            Equal(a.Used(), (isize)28, "%ld"); // 4*7
        }

        TEST_CASE("operator+: Append more than capacity")
        {
            Arena a = perm;

            Si32 s = {&a, 0};

            // s + 2; // Will print error and drop
            //           [E] Overflow: len + 1 (3) <= cap (2)
            //           Dropping item
            //           TODO: Any way of silencing the error?

            TCheck(s.len == s.cap);
        }

        TEST_CASE("operator[i]: By reference")
        {
            Arena a = perm;

            Si32 s = {&a, 3};                // Init
            RANGE(i, s.cap) { s + (i + 1); } // Append

            *s[0] = 10;          // Mutate
            TCheck(*s[0] == 10); // Mutated value

            // Positive indices
            TCheck(*s[0] == 10);
            TCheck(*s[1] == 2);
            TCheck(*s[2] == 3);

            // Negative indices
            TCheck(*s[-1] == 3);
            TCheck(*s[-2] == 2);
            TCheck(*s[-3] == 10);

            // Assert errors
            // TCheck(*s[-4] == 20);
            // TCheck(*s[3] == 20);
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
            TCheck(*sub[0] == *s[5]);

            // Mutate value in sub and original
            *sub[1] = 53;
            TCheck(*sub[1] == 53); // Mutated value in subslice
            TCheck(*s[6] == 53);   // Mutated origial

            // Reinit
            a = perm;
            s = {&a, 10};
            RANGE(i, s.cap) { s + i; }

            // Equal indices
            sub = s[1, 1];
            TCheck(sub.len == 0);

            sub = s[-1, -1];
            TCheck(sub.len == 0);

            // From end to end
            sub = s[-2, -1];
            TCheck(sub.len == 1);

            // From middle to end
            sub = s[s.len / 2, -1];
            TCheck(sub.len == (s.len / 2) - 1);

            // From middle to +-0
            sub = s[s.len / 2, -0];
            TCheck(sub.len == 0);

            sub = s[s.len / 2, 0];
            TCheck(sub.len == 0);
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
            TCheck(*sub[0] == *s[5]);

            // Mutate value in sub and original
            *sub[1] = 53;
            TCheck(*sub[1] == 53); // Mutated value in subslice
            TCheck(*s[6] == 6);    // Retains original value

            // Take subslice - negative indices
            auto sub1 = s[-8, -5, &a];
            auto sub2 = s[2, 5, &a];
            TCheck(sub1 == sub2);
        }

        TEST_CASE("operator==: Rock solid behaviour")
        {
            auto s0 = Si32({1, 2});

            // NOTE: Had to keep track of lengths manually, can we use `size`?
            auto s1 = Si32({1, 2});
            auto s2 = Si32({3, 4});
            auto s3 = Si32({3, 4});
            auto s4 = Si32({4, 3});
            auto s5 = Si32({3, 4, 5});
            auto s6 = Si32({3});
            auto s7 = Si32({});
            Si32 s8 = {0, 0, 0}; // Explicit 0 for buf

            // Self Equality
            TCheck(s1 == s1);
            TCheck(s2 == s2);
            TCheck(s6 == s6);

            // Zero equality
            TCheck(s7 == Si32((i32[]){}, 0, 0));

            // Null equality
            TCheck(s8 == s8);

            // Null zero equality
            TCheck(s8 == s7);

            // Cross Equality
            TCheck(s2 == s3);

            // Subset inequality
            TCheck(s2 != s6);

            // Superset inequality
            TCheck(s2 != s5);

            // Zero inequality
            TCheck(s2 != s7);

            // Null inequality
            TCheck(s2 != s8);
        }
    }

    return 0;
}
