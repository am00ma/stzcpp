#pragma once

#include "str.h"

template <typename V> struct Trie {

    Trie<V>* child[4] = {};

    Str key = ""; // Also gravestone?
    V   val = 0;  // Do we need a pointer?

    /* ---------------------------------------------------------------------------
     * Initialization - Usual zero init
     * ------------------------------------------------------------------------- */

    /* ---------------------------------------------------------------------------
     * Lookup and Insert
     * ------------------------------------------------------------------------- */

    V* Insert(Str key, Arena* a, bool store_key = false)
    {

        Trie<V>*  buf[1] = {this};
        Trie<V>** m      = buf;

        for (auto h = key.Hash64(); *m; h <<= 2)
        {
            if (key == (*m)->key) { return &(*m)->val; } // Found
            else { m = &(*m)->child[h >> 62]; }          // Check child
        }

        if (!a) { return 0; } // No insert, only get

        *m = a->Make<Trie<V>>(); // Create new map

        if (store_key)
        {
            (*m)->key = key.Copy(a); // Owning version - copy to arena
        }
        else
        {
            (*m)->key = key; // Non-owning version
        }

        return &(*m)->val; // Val for set/get
    }

    V* Lookup(Str key)
    {
        Trie<V>*  buf[1] = {this};
        Trie<V>** m      = buf;
        for (auto h = key.Hash64(); *m; h <<= 2)
        {
            if (key == (*m)->key) { return &(*m)->val; } // Found
            else { m = &(*m)->child[h >> 62]; }          // Check child
        }
        return 0;
    }

    Str* OrigKey(Str key)
    {
        Trie<V>*  buf[1] = {this};
        Trie<V>** m      = buf;
        for (auto h = key.Hash64(); *m; h <<= 2)
        {
            if (key == (*m)->key) { return &(*m)->key; } // Found
            else { m = &(*m)->child[h >> 62]; }          // Check child
        }
        return 0;
    }
};
