#include "dict.h"
#include "doctest.h"
#include "range.h"

int main()
{

    TEST_SUITE("LTrie")

    typedef Dict<Str, Str> SDict;

    Str testkeys[] = {"hello",  "hi",  "how",  "are",  "you",  //
                      "hello1", "hi1", "how1", "are1", "you1", //
                      "hello2", "hi2", "how2", "are2", "you2", //
                      "hello3", "hi3", "how3", "are3", "you3"};

    constexpr isize num_keys = sizeof(testkeys) / sizeof(Str);

    TEST_CASE("Initialization: from arena")
    {
        Arena a = {1024 * 4};

        SDict d = {&a, num_keys};

        RANGE(i, num_keys)
        {
            //
            *d[testkeys[i]] = testkeys[i];
        }

        // Iterator through keys
        RANGE(i, num_keys)
        {
            Str key = d.data[i]->key;
            Str val = d.data[i]->val;

            // Val by Lookup
            Str* ret = d[key, true];
            TNotNull(ret);
            TEqualStr(*ret, val);

            // Point to same mem
            TEqualAddr(ret->buf, val.buf);
        }
    }

    TEST_RESULTS();
}
