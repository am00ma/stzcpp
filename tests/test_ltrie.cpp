#include "ltrie.h"
#include "range.h"
#include <cassert>
#include <cstdio>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_SUITE("LTrie")
{
    typedef LTrie<Str>      STrie;
    typedef LTrieChild<Str> STrieChild;

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

    TEST_CASE("Algorithm")
    {
        Arena  a    = perm;
        STrie  trie = STrie(&a, 25);
        Arena* temp = &a;
        printf("\n");

        RANGE(i, 10)
        {
            STrieChild*  buf[1] = {&trie.root};
            STrieChild** m      = buf;

            Str   key    = testkeys[i];
            u64   hash   = testkeys[i].Hash64();
            isize offset = 0;

            while (*m)
            {
                // Found key
                if (key == buf[0]->key) { break; }

                // Compute offset
                offset = hash >> 62;
                assert((offset >= 0) && (offset <= 3));

                // Proceed to child
                m = &(*m)->child[offset];

                hash <<= 2;
            }

            // Got null child, alloc memory and insert
            *m        = temp->Make<STrieChild>();
            (*m)->key = key;
        }
    }

    TEST_CASE("Usage: Typical usage")
    {
        Arena a    = perm;
        STrie trie = STrie(&a, 25);
        Str*  ret  = {};

        // *trie["hello", &a] = "hi";
        // *trie["how", &a]   = "are";
        // *trie["you", &a]   = "";

        // RANGE(i, 20) { *trie[testkeys[i], &a] = testkeys[i]; }
        // trie.Print("After insert");

        // CHECK(*trie["hello"] == Str("hi"));
        // CHECK(*trie["how"] == Str("are"));
        //
        // // Can store empty string as well
        // ret = trie["you"];
        // CHECK(ret != 0);
        // CHECK(*ret == Str(""));
        //
        // // Check failed lookup
        // ret = trie["today"];
        // CHECK(ret == 0);
        //
        // RANGE(i, 20) { *trie[testkeys[i]] = testkeys[i]; }
        // CHECK(trie.len == 23);
        //
    }
}
