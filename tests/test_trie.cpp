#include "range.h"
#include "trie.h"
#include <cassert>
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
        Arena* temp = &a;
        printf("\n");

        RANGE(i, 1)
        {
            STrie*  now[1]  = {};
            STrie** nowstar = 0;
            now[0]          = &trie;
            *nowstar        = &trie;
            printf("1. %2ld | %16p \n", i, (void*)now[0]);

            Str   key    = testkeys[i];
            u64   hash   = testkeys[i].Hash64();
            isize offset = 0;

            while (now[0] != 0)
            {
                // Found key
                if (key == now[0]->key) { break; }

                // Compute offset
                offset = hash >> 62;
                assert((offset >= 0) && (offset <= 3));

                // Proceed to child
                now[0] = now[0]->child[offset];
                printf("2. %2ld | %16p \n", i, (void*)now[0]);

                hash <<= 2;
            }

            // Got null child, alloc memory and insert
            now[0]      = &(*temp->Make<STrie>());
            now[0]->key = key;
            printf("3. %2ld | %16p \n", i, (void*)now[0]);

            // // Got null child, alloc memory and insert
            // old->child[offset]      = ins;
            // old->child[offset]->key = key;

            RANGE(j, 4) { printf("-- %2ld | %16p \n", j, (void*)trie.child[j]); }
        }
    }

    TEST_CASE("Struct size")
    {
        CHECK(sizeof(STrie) == 64); // 8*4=32(*child) + 16(Str) + 16(V==Str)
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
    //     Arena a    = perm;
    //     STrie trie = {};
    //
    //     Str key = "hello";
    //     Str val = "hi";
    //
    //     // Insert key, val
    //     *trie.Upsert(key, &a) = val;
    //
    //     // Lookup
    //     Str* ret = trie.Upsert(key, &a);
    //
    //     // CHECK(ret == Str("hi"));
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
