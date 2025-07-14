#include "range.h"
#include "trie.h"
#include <cstdio>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_SUITE("Trie")
{
    typedef Trie<Str> STrie;

    Str testkeys[] = {"hello",  "hi",  "how",  "are",  "you",  //
                      "hello1", "hi1", "how1", "are1", "you1", //
                      "hello2", "hi2", "how2", "are2", "you2", //
                      "hello3", "hi3", "how3", "are3", "you3"};

    Arena a = Arena(1024 * 1024); // 1KB

    TEST_CASE("Algorithm")
    {
        STrie  trie = {};
        STrie* m    = &trie;

        Arena* temp = &a;

        printf("\n");
        printf("%7s | %16s | %16s | %5s \n", "key", "hash", "hash << 2", "child");

        RANGE(i, 5)
        {
            auto key  = testkeys[i];
            auto hash = testkeys[i].Hash64();

            printf("%7.*s | %016lX | %016lX | %5ld \n", pstr(key), hash, hash << 2, (hash << 2) >> 62);

            printf("Iterating: %p\n", m);
            for (auto h = key.Hash64(); m; hash <<= 2)
            {
                if (key == m->key)
                {
                    // Found
                    printf("Found \n");
                    goto out;
                }
                m = m->child[hash >> 62];
                printf("Iterating: %p\n", m);
            }

            if (!temp)
            {
                printf("Not found \n");
                goto out;
            }

            // New key val pair
            m      = temp->Make<STrie>();
            m->key = key;
            printf("New %p \n", m);

        out:
        }
    }

    TEST_CASE("Struct size")
    {
        CHECK(sizeof(Trie<Str>) == 64); // 8*4=32(*child) + 16(Str) + 16(V==Str)
    }

    Arena perm = Arena(1024); // 1KB

    TEST_CASE("Initialization")
    {
        STrie trie = {}; // zero-initialized
        CHECK(trie.key.len == 0);
        CHECK(trie.val.len == 0);
    }

    // TEST_CASE("Insert, Lookup: Successful")
    // {
    //     Arena a       = perm;
    //     isize cap_exp = 4;
    //     Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full
    //
    //     Str key = "hello";
    //     Str val = "hi";
    //
    //     *map.Insert(key) = val;
    //
    //     Str* ret = map.Lookup(key);
    //     CHECK(*ret == val);
    // }

    // TEST_CASE("Lookup: Unsuccessful TODO: API is wild currently")
    // {
    //     Arena a       = perm;
    //     isize cap_exp = 4;
    //     Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full
    //
    //     Str key = "hello";
    //     Str val = "hi";
    //
    //     *map.Insert(key) = val;
    //
    //     Str* ret = map.Lookup("how");
    //     CHECK(ret == 0);
    // }
    //
    // TEST_CASE("Insertion: Original key, original val")
    // {
    //     Arena a       = perm;
    //     isize cap_exp = 4;
    //     Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full
    //
    //     Str key = "hello";
    //     Str val = "hi";
    //
    //     *map.Insert(key) = val;
    //
    //     Str* ret = map.Lookup(key);
    //     CHECK(*ret == val);
    // }
    //
    // TEST_CASE("Insertion: Different keys, original val")
    // {
    //     Arena a       = perm;
    //     isize cap_exp = 4;
    //     Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full
    //
    //     Str key1 = "hello";
    //     Str key2 = "hello";
    //     Str val  = "hi";
    //
    //     *map.Insert(key1) = val;
    //
    //     Str* ret = map.Lookup(key2);
    //     CHECK(*ret == val);
    // }
    //
    // TEST_CASE("Insertion: More than max elements")
    // {
    //     Arena a       = perm;
    //     isize cap_exp = 4;
    //     Map   map     = Map<Str>(&a, cap_exp); // 2^4 = 16 elements when full
    //
    //     RANGE(i, (1 << cap_exp) + 1)
    //     {
    //         Str* ret = map.Insert(testkeys[i]);
    //         if (ret) { *ret = "hi"; }
    //     }
    //
    //     Str* ret = map.Lookup(testkeys[0]);
    //     CHECK(*ret == Str("hi"));
    // }

    // TODO: Other types as values
    // TODO: Null value handling
    // TODO: Processing time and metrics
}
