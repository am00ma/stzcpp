#include "arena.h"
#include "doctest.h"

template <typename V> struct s3 {
    i32 a;
    V   e[];
};

template <typename V> struct s4 {
    i32 a;
    V   e[3];
};

int main()
{
    TEST_SUITE("Arena")
    {

        TEST_CASE("Deps: alignof, sizeof")
        {
            TEqual(sizeof(char), (isize)1, "%ld");
            TEqual(alignof(char), (isize)1, "%ld");

            TEqual(sizeof(u16), (isize)2, "%ld");
            TEqual(alignof(u16), (isize)2, "%ld");

            TEqual(sizeof(u32), (isize)4, "%ld");
            TEqual(alignof(u32), (isize)4, "%ld");

            TEqual(sizeof(u64), (isize)8, "%ld");
            TEqual(alignof(u64), (isize)8, "%ld");

            TEqual(sizeof(i16), (isize)2, "%ld");
            TEqual(alignof(i16), (isize)2, "%ld");

            TEqual(sizeof(i32), (isize)4, "%ld");
            TEqual(alignof(i32), (isize)4, "%ld");

            TEqual(sizeof(i64), (isize)8, "%ld");
            TEqual(alignof(i64), (isize)8, "%ld");

            struct s1 {
                u16 a, b, c;
            };

            TEqual(sizeof(s1), (isize)6, "%ld");
            TEqual(alignof(s1), (isize)2, "%ld");

            struct s2 {
                u16  a, b, c;
                char d[];
            };

            TEqual(sizeof(s2), (isize)6, "%ld");
            TEqual(alignof(s2), (isize)2, "%ld");

            // template <typename V> struct s3 {
            //     u32  a;
            //     char d[];
            //     V    e[];
            // };
            // --> Errors with
            // Flexible array member 'd' with type 'char[]' is not at the end of struct [flexible_array_not_at_end]

            // template <typename V> struct s3 {
            //     u32  a;
            //     V    e[];
            // };
            // --> Has to be defined in global namespace?
            // 1. Templates can only be declared in namespace or class scope [template_outside_namespace_or_class_scope]
            TEqual(sizeof(s3<char>), (isize)4, "%ld");
            TEqual(alignof(s3<char>), (isize)4, "%ld");

            // Passes, as V[] is not included in size of struct
            TEqual(sizeof(s3<i16>), (isize)4, "%ld");
            TEqual(alignof(s3<i16>), (isize)4, "%ld");

            // template <typename V> struct s4 {
            //     i32 a;
            //     V   e[3];
            // };
            // Passes, as V[] is not included in size of struct
            TEqual(sizeof(s4<i16>), (isize)12, "%ld"); // (i32 = 4) + (V* = 8)
            TEqual(alignof(s4<i16>), (isize)4, "%ld");

            struct s5 {
                i32 a;
                i16 b;
            };

            // NOTE: is not equal to 6 -> (i32 = 4) + (i16 = 2)
            TEqual(sizeof(s5), (isize)8, "%ld");
            TEqual(alignof(s5), (isize)4, "%ld");
        }

        TEST_CASE("Size: struct")
        {
            // The struct itself is very light, with 3 64-bit integers
            isize size = 24;
            TEqual(sizeof(Arena), size, "%ld"); // 8(beg) + 8(end) + 8(cap)
        }

        TEST_CASE("Malloc: up to 2^35, without free")
        {
            // It is possible to allot astonishing amount of memory,
            // all the way upto 4 EB (MB, GB, TB, PB, EB), that too, cumulatively
            // BUG: What is happening here? Does'nt reflect in btop
            isize cap = 1;
            RANGE(i, 63)
            {
                Arena a = {cap};
                TEqual(a.cap, cap, "%ld");

                cap *= 2;
            }
        }

        TEST_CASE("Malloc: up to 2^35, with free")
        {
            // BUG: If free is added, crashes at i=35 (32 GB)
            isize cap = 1;
            RANGE(i, 35)
            {
                Arena a = {cap};
                TEqual(a.cap, cap, "%ld");
                a.Free();

                cap *= 2;
            }
        }

        // Make arena once to borrow for each case
        Arena perm = {1024}; // 1 KB

        TEST_CASE("Alloc: Allocated sizes (well aligned)")
        {
            Arena a = perm;
            i32*  x = a.Make<i32>();
            TNotNull(x);
            TEqual(a.Used(), (isize)4, "%ld");
            TEqual(a.Available<i32>(), (isize)255, "%ld");
            TEqual(a.Available<i16>(), (isize)510, "%ld");
            TEqual(a.Available<char>(), (isize)1020, "%ld");

            // Zero count
            i32* y = a.Make<i32>(0);
            TNull(y);
            TEqual(a.Used(), (isize)4, "%ld");
            TEqual(a.Available<i32>(), (isize)255, "%ld");
            TEqual(a.Available<i16>(), (isize)510, "%ld");
            TEqual(a.Available<char>(), (isize)1020, "%ld");

            i16* z = a.Make<i16>(3);
            TNotNull(z);
            TEqual(a.Used(), (isize)10, "%ld");
            TEqual(a.Available<i32>(), (isize)253, "%ld"); // Alignment
            TEqual(a.Available<i16>(), (isize)507, "%ld");
            TEqual(a.Available<char>(), (isize)1014, "%ld");
        }

        TEST_CASE("Alloc: Allocated sizes (half aligned)")
        {
            Arena a = perm;
            i16*  x = a.Make<i16>();
            TEqual(a.Used(), (isize)2, "%ld");
        }

        TEST_CASE("Alloc: Zeroed for primitives")
        {
            Arena a = perm;
            i32*  y = a.Make<i32>(3);
            RANGE(i, 3) { TEqual(y[i], 0, "%d"); }
        }

        // Arbitrary datatype
        typedef struct Item {
            i32 a = 4;
            i32 b = 8;

            Item* Copy(Arena* a)
            {
                Item* val = a->Make<Item>();
                return val;
            }

        } Item;

        TEST_CASE("Alloc: Zeroed for structs")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(3);
            RANGE(i, 3)
            {
                TEqual(y[i].a, 0, "%d");
                TEqual(y[i].b, 0, "%d");
            }
        }

        TEST_CASE("Alloc: Elements with defaults")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(3, DEFAULTS);
            RANGE(i, 3)
            {
                TEqual(y[i].a, 4, "%d");
                TEqual(y[i].b, 8, "%d");
            }
        }

        TEST_CASE("Alloc: Elements with overriden defaults")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(3, DEFAULTS, 3, 5); // Have to get order of new correct
            RANGE(i, 3)
            {
                TEqual(y[i].a, 3, "%d");
                TEqual(y[i].b, 5, "%d");
            }
        }

        TEST_CASE("Alloc: Non-zeroed")
        {
            Arena a = perm;

            // Values to overwrite
            Arena b   = a;
            Item* tmp = b.Make<Item>(3, DEFAULTS);

            // No memset (now using Arena a, so will overwrite Arena b)
            Item* y = a.Make<Item>(3, NOZERO);
            RANGE(i, 3)
            {
                // Returns results from prev arena,
                // so we can assert the value
                TEqual(y[i].a, 4, "%d");
                TEqual(y[i].b, 8, "%d");
            }
        }

        TEST_CASE("Alloc: Soft-fail")
        {
            Arena a = perm;
            Item* y = a.Make<Item>(1024, SOFTFAIL);
            TEqual(y, (Item*)0, "%p");
        }

        TEST_CASE("TODO: Alloc: Non-aligned") { TCheck(0 == 0); }
        TEST_CASE("TODO: Alloc: Threaded") { TCheck(0 == 0); }
    }

    return 0;
}
