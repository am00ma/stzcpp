#include "dict.h"
#include "doctest.h"
#include "log.h"
#include "range.h"

int main()
{

    TEST_SUITE("Dict")

    typedef Dict<Str, Str> SDict;

    Str testkeys[] = {"hello",  "hi",  "how",  "are",  "you",  //
                      "hello1", "hi1", "how1", "are1", "you1", //
                      "hello2", "hi2", "how2", "are2", "you2", //
                      "hello3", "hi3", "how3", "are3", "you3"};

    constexpr isize num_keys = sizeof(testkeys) / sizeof(Str);

    Arena perm = {1024 * 4};

    TEST_CASE("Initialization: from arena")
    {
        // Initialize an arena
        Arena a = perm;

        // Store some keys and values
        SDict d = {&a, num_keys};
        RANGE(i, num_keys) { *d[testkeys[i]] = testkeys[i]; }

        RANGE(i, num_keys)
        {
            // Iterator through items in order
            Str key = d.data[i]->key;
            Str val = d.data[i]->val;
            TEqualStr(val, testkeys[i]);

            // Val by Lookup
            Str* ret = d[key, false];
            TNotNull(ret);
            TEqualStr(*ret, val);
            TEqualAddr(ret->buf, val.buf); // Point to same mem
        }

        TEqualLong(a.Used(), 1280); // 20 items (64 bytes per item)
    }

    TEST_CASE("Reallocation:")
    {
        // pointers will change, but if underlying data is ok, all is ok

        Arena a = perm;
        SDict d = {&a, 5};
        RANGE(i, 5) { *d[testkeys[i]] = testkeys[i]; }
        TEqualLong(d.data.len, 5);
        TEqualLong(d.data.cap, 5);
        TEqualLong(a.Used(), 320); // 5 items (64 bytes per item)

        // Dynamic insertion
        RANGE(i, 5, 10) { *d[testkeys[i], true, &a] = testkeys[i]; }
        TEqualLong(d.data.len, 10);
        TEqualLong(d.data.cap, 10);
        TEqualLong(a.Used(), 640); // 320 * 2

        *d[testkeys[10], true, &a] = testkeys[10];
        *d[testkeys[11], true, &a] = testkeys[11];
        TEqualLong(d.data.len, 12);
        TEqualLong(d.data.cap, 20);
        TEqualLong(a.Used(), 1280); // 320 * 2 * 2

        *d[testkeys[12], true, &a] = testkeys[12];
        TEqualLong(a.Used(), 1280);

        RANGE(i, d.data.len)
        {
            // Iterator through items in order
            Str key = d.data[i]->key;
            Str val = d.data[i]->val;
            TEqualStr(val, testkeys[i]);

            // Val by Lookup
            Str* ret = d[key, false];
            TNotNull(ret);
            TEqualStr(*ret, val);
            TEqualAddr(ret->buf, val.buf); // Point to same mem
        }
    }

    TEST_RESULTS();
}
