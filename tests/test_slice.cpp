#include "slice.h"

#include "doctest.h"

TEST_SUITE("Slice")
{

    typedef struct Item {
        i32 a = 4;
        i32 b = 8;
    } Item;

    Arena perm = Arena(1024); // 1 KB

    TEST_CASE("Stuct size")
    {
        CHECK(sizeof(Slice<i32>) == 24);  // 8(buf) + 8(len) + 8(cap)
        CHECK(sizeof(Slice<Item>) == 24); // independent of value type
    }

    TEST_CASE("Append")
    {
        Arena      a = perm;
        Slice<i32> s = Slice<i32>(&a, 2);
        s.Append(12);
        s.Append(24);
        CHECK(*s[0] == 12);
        CHECK(*s[1] == 24);
    }

    TEST_CASE("Item: By reference")
    {
        Arena      a = perm;
        Slice<i32> s = Slice<i32>(&a, 3);
        RANGE(i, s.cap) { s.Append(i); }
        RANGE(i, s.len) { *s[i] = i + 10; }
        CHECK(s.data[0] == 10);
    }

    TEST_CASE("Subslice: By reference")
    {
        Arena      a = perm;
        Slice<i32> s = Slice<i32>(&a, 10);
        RANGE(i, s.cap) { s.Append(i); }
        Slice<i32> sub = s[5, 8];
        CHECK(sub.len == 3);
        RANGE(i, sub.cap) { CHECK(*sub[i] == *s[5 + i]); }; // Check values
    }

    TEST_CASE("Subslice: By reference")
    {
        Arena      a = perm;
        Slice<i32> s = Slice<i32>(&a, 10);
        RANGE(i, s.cap) { s.Append(i); }
        auto sub = s[5, 8, &a];
        *sub[1]  = 53;
        CHECK(*sub[0] == *s[5]);
        CHECK(*sub[1] == 53);
        CHECK(*s[6] == 6);
        CHECK(*sub[2] == *s[7]);
    }
}
