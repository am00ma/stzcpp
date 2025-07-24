#pragma once

#include "log.h"
#include "str.h"
#include <cstdio>

template <typename V> struct LTrieChild {
    LTrieChild<V>* child[4] = {};

    Str key = "";
    V   val = 0;

    void Print() { printf("%.*s -> %.*s \n", pstr(key), pstr(val)); }

    void PrintChildren()
    {
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

    V* operator[](Str key, bool insert = false)
    {
        LTrieChild<V>*  buf[1] = {&root};
        LTrieChild<V>** m      = buf;

        for (auto h = key.Hash64(); *m; h <<= 2)
        {
            if (key == (*m)->key) { goto out; } // Found
            else { m = &(*m)->child[h >> 62]; } // Check child
        }

        // No insert, so return not found (len does not increment)
        if (!insert) { return 0; }

        // We have a new key -> allocate space for new map
        if (len >= cap - 1) { Fatal(-1, "Exceeded capacity: %ld\n", cap); }

        *m = &data[len];
        len++;

    out:

        (*m)->key = key;
        return &(*m)->val; // Val for set/get
    }

    void Print(const char* label, bool children = false)
    {
        printf("%s: len: %ld, cap: %ld, data: %p\n", label, len, cap, (void*)data);
        RANGE(i, len)
        {
            printf("%ld: ", i);
            data[i].Print(); // Print children
            if (children) { data[i].PrintChildren(); }
        }
    }
};
