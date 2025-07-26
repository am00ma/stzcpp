#include "ltrie.h"
#include "range.h"

#include "doctest.h"

int main()
{

    TEST_SUITE("LTrie")
    {
        typedef LTrie<Str> STrie;

        Str testkeys[] = {"hello",  "hi",  "how",  "are",  "you",  //
                          "hello1", "hi1", "how1", "are1", "you1", //
                          "hello2", "hi2", "how2", "are2", "you2", //
                          "hello3", "hi3", "how3", "are3", "you3"};

        Arena perm = Arena(1024 * 1024); // 1MB

        TEST_CASE("Initialization")
        {
            Arena a    = perm;
            STrie trie = STrie(&a, 25); // zero-initialized
        }

        TEST_CASE("Usage: Typical usage")
        {
            Arena a    = perm;
            STrie trie = STrie(&a, 25);
            Str*  ret  = {};

            *trie["hello", true] = "hi";

            ret = trie["hello"];
            CHECK(*ret == Str("hi"));

            *trie["how", true] = "are";

            ret = trie["how"];
            CHECK(*ret == Str("are"));

            *trie["you", true] = "";

            ret = trie["you"];
            CHECK(*ret == Str(""));
        }

        TEST_CASE("Insert, Lookup: Successful, unsuccessful lookup")
        {
            Arena a    = perm;
            STrie trie = STrie(&a, 25);
            Str*  ret  = {};

            // Insert
            RANGE(i, 10) { *trie[testkeys[i], true] = testkeys[i]; }

            // Successful lookups
            RANGE(i, 10)
            {
                ret = trie[testkeys[i]];
                CHECK(*ret == testkeys[i]);
            }

            // Unsuccessful lookups
            RANGE(i, 10, 20)
            {
                ret = trie[testkeys[i]];
                CHECK(ret == 0);
            }
        }

        TEST_CASE("Iteration: Typical usage")
        {
            Arena a    = perm;
            STrie trie = STrie(&a, 25);
            Str*  ret  = {};

            // Insert
            RANGE(i, 10) { *trie[testkeys[i], true] = testkeys[i]; }

            // Iterate over keys and values
            RANGE(i, trie.len)
            {
                CHECK(trie.data[i].key == testkeys[i]);
                CHECK(trie.data[i].val == testkeys[i]);
            }
        }
    }

    TEST_RESULTS();

    return 0;
}
