#include "map.h"
#include <cstdio>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

void map_print(Map<Str> map, Str key)
{
    // Lookup when called with 0 as lhs
    Str* val = map.Lookup(key);

    // Check val->buf for valid Str
    if (val->buf) printf("%.*s : %.*s\n", pstr(key), ppstr(val));
    else printf("Not found: %.*s\n", pstr(key));
}

TEST_SUITE("Map")
{
    TEST_CASE("Struct size")
    {
        CHECK(sizeof(Map<Str>) == 24); // 8(keys) + 8(vals) + 8(cap_exp)
    }

    Arena a = Arena(1024); // 1KB

    TEST_CASE("Initialization")
    {
        Arena temp    = a;
        isize cap_exp = 4;
        Map   map     = Map<Str>(&temp, cap_exp); // 2^4 = 16 elements when full
        CHECK(map.cap_exp == 4);
    }

    TEST_CASE("Lookup: Successful")
    {
        Arena temp    = a;
        isize cap_exp = 4;
        Map   map     = Map<Str>(&temp, cap_exp); // 2^4 = 16 elements when full

        Str key = "hello";
        Str val = "hi";

        *map.Lookup(key) = val;

        Str* ret = map.Lookup(key);
        CHECK(*ret == val);
    }

    TEST_CASE("Lookup: Unsuccessful TODO: API is wild currently")
    {
        Arena temp    = a;
        isize cap_exp = 4;
        Map   map     = Map<Str>(&temp, cap_exp); // 2^4 = 16 elements when full

        Str key = "hello";
        Str val = "hi";

        *map.Lookup(key) = val;

        Str ret = *map.Lookup("how");
        CHECK(ret.buf == 0);
    }

    TEST_CASE("Insertion: Original key, original val")
    {
        Arena temp    = a;
        isize cap_exp = 4;
        Map   map     = Map<Str>(&temp, cap_exp); // 2^4 = 16 elements when full

        Str key = "hello";
        Str val = "hi";

        *map.Lookup(key) = val;

        Str* ret = map.Lookup(key);
        CHECK(*ret == val);
    }

    TEST_CASE("Insertion: Different keys, original val")
    {
        Arena temp    = a;
        isize cap_exp = 4;
        Map   map     = Map<Str>(&temp, cap_exp); // 2^4 = 16 elements when full

        Str key1 = "hello";
        Str key2 = "hello";
        Str val  = "hi";

        *map.Lookup(key1) = val;

        Str* ret = map.Lookup(key2);
        CHECK(*ret == val);
    }
}
