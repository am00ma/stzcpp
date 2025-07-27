#include "arena.h"
#include "doctest.h"

int main()
{

    TEST_SUITE("Arena")
    {

        TEST_CASE("Stuct size")
        {
            // The struct itself is very light, with 3 64-bit integers
            isize size = 24;
            Equal(sizeof(Arena), size, "%ld"); // 8(beg) + 8(end) + 8(cap)
        }

        TEST_CASE("malloc up to 2^35, without free")
        {
            // It is possible to allot astonishing amount of memory,
            // all the way upto 4 EB (MB, GB, TB, PB, EB), that too, cumulatively
            // BUG: What is happening here? Does'nt reflect in btop
            isize cap = 1;
            RANGE(i, 63)
            {
                Arena a = {cap};
                Equal(a.cap, cap, "%ld");

                cap *= 2;
            }
        }

        TEST_CASE("malloc up to 2^35, with free")
        {
            // BUG: If free is added, crashes at i=35 (32 GB)
            isize cap = 1;
            RANGE(i, 35)
            {
                Arena a = {cap};
                Equal(a.cap, cap, "%ld");
                a.Free();

                cap *= 2;
            }
        }

        // Make arena once to borrow for each case
        Arena perm = {1024}; // 1 KB

        TEST_CASE("Allocated sizes")
        {
            Arena a = perm;
            i32*  x = a.Make<i32>();
            Equal(a.Used(), (isize)4, "%ld");
        }

        TEST_CASE("Zeroed Initialization for primitives")
        {
            Arena a = perm;
            i32*  y = a.Make<i32>(3);
            RANGE(i, 3) { Equal(y[i], 0, "%d"); }
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
                Equal(y[i].a, 0, "%d");
                Equal(y[i].b, 0, "%d");
            }
        }

        TEST_CASE("Elements with defaults")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(3, DEFAULTS);
            RANGE(i, 3)
            {
                Equal(y[i].a, 4, "%d");
                Equal(y[i].b, 8, "%d");
            }
        }

        TEST_CASE("Elements with default args, but overriden")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(3, DEFAULTS, 3, 5); // Have to get order of new correct
            RANGE(i, 3)
            {
                Equal(y[i].a, 3, "%d");
                Equal(y[i].b, 5, "%d");
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
                NotEqual(y[i].a, 0, "%d");
                NotEqual(y[i].b, 0, "%d");
            }
        }

        // For contrast, after above
        TEST_CASE("Zeroed Initialization")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(3);
            RANGE(i, 3)
            {
                Equal(y[i].a, 0, "%d");
                Equal(y[i].b, 0, "%d");
            }
        }

        TEST_CASE("Soft-fail")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(1024, SOFTFAIL);
            Equal(y, (Item*)0, "%p");
        }

        TEST_CASE("TODO: Non-aligned access") { CHECK(0 == 0); }
        TEST_CASE("TODO: Multiple threads") { CHECK(0 == 0); }
    }

    return 0;
}
