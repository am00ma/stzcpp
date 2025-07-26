#include "map.h"
#include "range.h"
#include <cstdio>

#include "doctest.h"

int main()
{

    TEST_SUITE("Map")
    {
        Str testkeys[] = {"hello",  "hi",  "how",  "are",  "you",  //
                          "hello1", "hi1", "how1", "are1", "you1", //
                          "hello2", "hi2", "how2", "are2", "you2", //
                          "hello3", "hi3", "how3", "are3", "you3"};

        TEST_CASE("Algorithm")
        {
            isize cap_exp = 4;

            Arena a    = Arena(1024 * 1024);        // 1KB
            Str*  keys = a.Make<Str>(1 << cap_exp); // Default = 4 (16 elements)

            // printf("\n");
            // printf(" %3s | %11s | %17s | %8s | %5s | %5s | %4s | %4s | %4s |\n", //
            //        "idx", "key", "hash", "cap", "mask", "shift", "hash>>shift", "step", "step(decimal)");

            RANGE(j, 1 << cap_exp)
            {
                Str key  = testkeys[j];
                u64 hash = testkeys[j].Hash64();
                u64 mask = (1 << cap_exp) - 1;
                u64 step = (hash >> (64 - cap_exp)) | 1;

                // printf(" %3ld |  %10.*s |  %016lX |  %0lX |  %0lX |  %0ld |  %02lX |  %02lX | %ld | ", //
                //        j, pstr(key), hash, (u64)(1 << cap_exp), mask, (64 - cap_exp), hash >> (64 - cap_exp), step,
                //        step);

                for (i64 i = hash;;)
                {
                    i = (i + step) & mask;
                    // printf("%ld, ", i);
                    if (!keys[i].buf)
                    {
                        keys[i] = key;
                        // printf("New\n");
                        goto out;
                    }
                    else if (keys[i] == key)
                    {
                        // printf("Found\n");
                        goto out;
                    } // Equals: O(n)
                }
            out:
            }
            // printf("\n");
        }

        TEST_CASE("Struct size")
        {
            CHECK(sizeof(Map<Str>) == 32); // 8(keys) + 8(vals) + 8(cap_exp) + 8(len)
        }

        Arena perm = Arena(1024); // 1KB

        TEST_CASE("Initialization")
        {
            Arena a       = perm;
            isize cap_exp = 4;
            Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full
            CHECK(map.cap_exp == 4);
        }

        TEST_CASE("Insert, Lookup: Successful")
        {
            Arena a       = perm;
            isize cap_exp = 4;
            Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full

            Str key = "hello";
            Str val = "hi";

            *map.Insert(key) = val;

            Str* ret = map.Lookup(key);
            CHECK(*ret == val);
        }

        TEST_CASE("Lookup: Unsuccessful TODO: API is wild currently")
        {
            Arena a       = perm;
            isize cap_exp = 4;
            Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full

            Str key = "hello";
            Str val = "hi";

            *map.Insert(key) = val;

            Str* ret = map.Lookup("how");
            CHECK(ret == 0);
        }

        TEST_CASE("Insertion: Original key, original val")
        {
            Arena a       = perm;
            isize cap_exp = 4;
            Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full

            Str key = "hello";
            Str val = "hi";

            *map.Insert(key) = val;

            Str* ret = map.Lookup(key);
            CHECK(*ret == val);
        }

        TEST_CASE("Insertion: Different keys, original val")
        {
            Arena a       = perm;
            isize cap_exp = 4;
            Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full

            Str key1 = "hello";
            Str key2 = "hello";
            Str val  = "hi";

            *map.Insert(key1) = val;

            Str* ret = map.Lookup(key2);
            CHECK(*ret == val);
        }

        TEST_CASE("Insertion: More than max elements")
        {
            Arena a       = perm;
            isize cap_exp = 4;
            Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full

            RANGE(i, (1 << cap_exp) + 1)
            {
                Str* ret = map.Insert(testkeys[i]);
                if (ret) { *ret = "hi"; }
            }

            Str* ret = map.Lookup(testkeys[0]);
            CHECK(*ret == Str("hi"));
        }

        // TODO: Other types as values
        // TODO: Null value handling
        // TODO: Processing time and metrics
    }

    TEST_RESULTS();

    return 0;
}
