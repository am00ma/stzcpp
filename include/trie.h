#pragma once

#include "str.h"

template <typename V> struct Trie {

    Trie<V>* child[4] = {}; // TrieAry = 4

    Str key = ""; // Also gravestone?
    V   val = 0;  // Do we need a pointer?

    /* ---------------------------------------------------------------------------
     * Initialization - Usual zero init
     * ------------------------------------------------------------------------- */

    /* ---------------------------------------------------------------------------
     * Lookup and Insert
     * ------------------------------------------------------------------------- */

    V* Upsert(Str key, Arena* a)
    {
        auto m = &this;
        for (auto h = key.Hash64(); *m; h <<= 2)
        {
            if (key == (*m)->key) { return &(*m)->val; }
            m = &(*m)->child[h >> 62];
        }
        if (!a) { return 0; }
        *m     = a->Make<Trie>();
        *m.key = key;
        return &(*m)->val;
    }
};
