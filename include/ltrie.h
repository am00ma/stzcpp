#pragma once

#include "log.h"
#include "str.h"
#include <cstdio>

template <typename V> struct LTrieChild {
    LTrieChild<V>* child[4] = {};

    Str key = "";
    V   val = 0;

    void Print(const char* label, isize i)
    {
        printf("%ld %s: %.*s -> %.*s \n", i, label, pstr(key), pstr(val));
        printf("   0: %p \n", (void*)child[0]);
        printf("   1: %p \n", (void*)child[1]);
        printf("   2: %p \n", (void*)child[2]);
        printf("   3: %p \n", (void*)child[3]);
    }
};

template <typename V> struct LTrie {
    LTrieChild<V> root = {};

    isize len = 0;
    isize cap = 0;

    LTrieChild<V>* data;

    /* ---------------------------------------------------------------------------
     * Initialization - Usual zero init
     * ------------------------------------------------------------------------- */

    // From Arena
    LTrie(Arena* a, isize cap_)
    {
        cap  = cap_;
        data = a->Make<LTrieChild<V>>(cap);
    }

    /* ---------------------------------------------------------------------------
     * Lookup and Insert
     * ------------------------------------------------------------------------- */

    V* operator[](Str key, Arena* a = 0)
    {
        LTrieChild<V>*  buf[1] = {root};
        LTrieChild<V>** m      = buf;

        u64 hash = key.Hash64();

        for (auto h = hash; *m; h <<= 2)
        {
            if (key == (*m)->key) // Found
            {
                (*m)->key = key;
                return &(*m)->val; // Val for set/get
            }
            else { m = &(*m)->child[h >> 62]; } // Check child
        }

        // No insert, so return not found (len does not increment)
        if (!a) { return 0; }

        // We have a new key -> allocate space for new map
        if (len >= cap - 1) { Fatal(-1, "Exceeded capacity: %ld\n", cap); }

        *m        = &data[len];
        (*m)->key = key;
        len++;

        return &(*m)->val; // Val for set/get
    }

    void Print(const char* label)
    {
        printf("%s: len: %ld, cap: %ld, data: %p\n", label, len, cap, (void*)data);

        RANGE(i, len)
        {
            printf("%ld: %.*s -> %.*s \n", i, pstr(data[i].key), pstr(data[i].val));
            RANGE(j, 4) { data[i].Print("  ", j); }
        }
    }
};
