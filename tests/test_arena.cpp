#include "arena.h"

#include "doctest.h"

int main()
{

    TEST_SUITE("Arena")
    {

        TEST_CASE("Stuct size")
        {
            // The struct itself is very light, with 3 64 bit integers
            CHECK(sizeof(Arena) == 24); // 8(beg) + 8(end) + 8(cap)
        }

        TEST_CASE("malloc up to 2^35, without free")
        {
            // It is possible to allot astonishing amount of memory,
            // all the way upto 4 EB (MB, GB, TB, PB, EB), that too, cumulatively
            // BUG: What is happening here? Does'nt reflect in btop
            isize cap = 1;
            RANGE(i, 63)
            {
                Arena a = Arena(cap);
                CHECK(a.cap == cap);

                cap *= 2;
            }
        }

        TEST_CASE("malloc up to 2^35, with free")
        {
            // BUG: If free is added, crashes at i=35 (32 GB)
            isize cap = 1;
            // RANGE(i, 63)
            RANGE(i, 35)
            {
                Arena a = Arena(cap);
                CHECK(a.cap == cap);
                a.Free();

                cap *= 2;
            }
        }

        // Make arena once to borrow for each case
        Arena perm = Arena(1024); // 1 KB

        TEST_CASE("Allocated sizes")
        {
            Arena a = perm;
            i32*  x = a.Make<i32>();
            CHECK(a.Used() == 4);
        }

        TEST_CASE("Zeroed Initialization for primitives")
        {
            Arena a = perm;
            i32*  y = a.Make<i32>(3);
            RANGE(i, 3) { CHECK(y[i] == 0); }
        }

        // Arbitrary datatype
        typedef struct Item {
            i32 a = 4;
            i32 b = 8;
        } Item;

        TEST_CASE("Zeroed Initialization for structs")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(3);
            RANGE(i, 3)
            {
                CHECK(y[i].a == 0);
                CHECK(y[i].b == 0);
            }
        }

        TEST_CASE("Elements with defaults")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(3, DEFAULTS);
            RANGE(i, 3)
            {
                CHECK(y[i].a == 4);
                CHECK(y[i].b == 8);
            }
        }

        TEST_CASE("Elements with default args")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(3, DEFAULTS, 3, 5); // Have to get order of new correct
            RANGE(i, 3)
            {
                CHECK(y[i].a == 3);
                CHECK(y[i].b == 5);
            }
        }

        TEST_CASE("Non-zeroed Initialization")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(3, NOZERO);
            RANGE(i, 3)
            {
                // Returns results from prev arena,
                // so we can assert != 0
                CHECK(y[i].a != 0);
                CHECK(y[i].b != 0);
            }
        }

        // For contrast, after above
        TEST_CASE("Zeroed Initialization")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(3);
            RANGE(i, 3)
            {
                CHECK(y[i].a == 0);
                CHECK(y[i].b == 0);
            }
        }

        TEST_CASE("Soft-fail")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(1024, SOFTFAIL);
            CHECK(y == 0);
        }

        TEST_CASE("TODO: Non-aligned access") { CHECK(0 == 0); }
        TEST_CASE("TODO: Multiple threads") { CHECK(0 == 0); }
    }

    TEST_RESULTS();

    return 0;
}
