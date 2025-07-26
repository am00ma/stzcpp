#include "range.h"
#include "trie.h"

#include "doctest.h"

int main()
{

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

            RANGE(i, 10)
            {
                STrie*  buf[1] = {&trie};
                STrie** m      = buf;

                Str   key    = testkeys[i];
                u64   hash   = testkeys[i].Hash64();
                isize offset = 0;

                while (*m)
                {
                    // Found key
                    if (key == buf[0]->key) { break; }

                    // Compute offset
                    offset = hash >> 62;
                    CHECK((offset >= 0) && (offset <= 3));

                    // Proceed to child
                    m = &(*m)->child[offset];

                    hash <<= 2;
                }

                // Got null child, alloc memory and insert
                *m        = temp->Make<STrie>();
                (*m)->key = key;
            }
        }

        TEST_CASE("Struct size")
        {
            CHECK(sizeof(STrie) == 64); // 8*4=32(*child) + 16(Str) + 16(V==Str)
        }

        Arena perm = Arena(2048); // 2KB

        TEST_CASE("Initialization")
        {
            STrie trie = {}; // zero-initialized
            CHECK(trie.key.len == 0);
            CHECK(trie.val.len == 0);
        }

        TEST_CASE("Insert, Lookup: Successful")
        {
            Arena a    = perm;
            STrie trie = {};

            Str key = "hello";
            Str val = "hi";

            // Insert key, val
            *trie.Insert(key, &a) = val;

            // Lookup
            Str* ret = trie.Lookup(key);

            CHECK(*ret == Str("hi"));
        }

        TEST_CASE("Insert, Lookup: Unsuccessful")
        {
            Arena a    = perm;
            STrie trie = {};

            Str key = "hello";
            Str val = "hi";

            // Insert key, val
            *trie.Insert(key, &a) = val;

            // Lookup
            Str* ret = trie.Lookup("how");

            CHECK(ret == 0);
        }

        TEST_CASE("Insert, Lookup, OrigKey: Non-owning keys")
        {
            Arena a    = perm;
            STrie trie = {};

            Str key = "hello";
            Str val = "hi";

            *trie.Insert(key, &a, false) = val;

            Str* ret = trie.Lookup(key);
            CHECK(*ret == val);

            Str* orig = trie.OrigKey(key);
            CHECK(orig->len == key.len);
            CHECK(orig->buf == key.buf);
        }

        TEST_CASE("Insert, Lookup, OrigKey: Storing keys in arena")
        {
            Arena a    = perm;
            STrie trie = {};

            Str key = "hello";
            Str val = "hi";

            *trie.Insert(key, &a, true) = val;

            Str* ret = trie.Lookup(key);
            CHECK(*ret == val);

            Str* orig = trie.OrigKey(key);
            CHECK(orig->len == key.len);
            CHECK(orig->buf != key.buf);
        }

        TEST_CASE("Insert, Lookup: Operator")
        {
            Arena a    = perm;
            STrie trie = {};

            Str key = "hello";
            Str val = "hi";

            *trie[key, &a] = val;

            Str* ret = trie[key];
            CHECK(*ret == val);

            Str* orig = trie.OrigKey(key);
            CHECK(orig->len == key.len);
            CHECK(orig->buf == key.buf);
        }

        TEST_CASE("Usage: Typical usage")
        {
            Arena a    = perm;
            STrie trie = {};
            Str*  ret  = {};

            *trie["hello", &a] = "hi";
            *trie["how", &a]   = "are";
            *trie["you", &a]   = "";

            CHECK(*trie["hello"] == Str("hi"));
            CHECK(*trie["how"] == Str("are"));

            // Can store empty string as well
            ret = trie["you"];
            CHECK(ret != 0);
            CHECK(*ret == Str(""));

            // Check failed lookup
            ret = trie["today"];
            CHECK(ret == 0);

            // Check 'owning status' of key
            Str* orig;
            Str  key = "the key string";

            *trie[key, &a, false] = "is not in arena";

            orig = trie.OrigKey(key);
            CHECK(orig->len == key.len);
            CHECK(orig->buf == key.buf);

            *trie[key, &a, true] = "is in arena";

            orig = trie.OrigKey(key);
            CHECK(orig->len == key.len);
            CHECK(orig->buf != key.buf);
        }

        TEST_CASE("Iteration: Typical usage")
        {
            Arena a    = perm;
            STrie trie = {};
            RANGE(i, 20) { *trie[testkeys[i], &a] = testkeys[i]; }
            RANGE(i, 20) { CHECK(*trie[testkeys[i]] == testkeys[i]); }

            // for key, val in trie.items(): ...
        }
        // TODO: Iteration
        // TODO: Other types as values
        // TODO: Processing time and metrics
    }

    TEST_RESULTS();

    return 0;
}
