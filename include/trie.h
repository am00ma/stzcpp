#pragma once

#include "str.h"

template <typename V> struct Trie {

    Trie<V>* child[4] = {};

    Str key = ""; // Also gravestone?
    V   val = {}; // Do we need a pointer?

    /* ---------------------------------------------------------------------------
     * Initialization - Usual zero init?
     * ------------------------------------------------------------------------- */
    Trie() = default;

    /* ---------------------------------------------------------------------------
     * Lookup and Insert
     * ------------------------------------------------------------------------- */

    // Copy of Insert
    V* operator[](Str key, Arena* a = 0, bool store_key = false)
    {
        Trie<V>*  buf[1] = {this};
        Trie<V>** m      = buf;
        for (auto h = key.Hash64(); *m; h <<= 2)
        {
            if (key == (*m)->key) { goto out; } // Found
            else { m = &(*m)->child[h >> 62]; } // Check child
        }

        if (!a) { return 0; } // No insert, so return not found

        *m = a->Make<Trie<V>>(); // Create new map

    // Expected behaviour is that if we call second time with store_key==true,
    // it should store it. Therefore, we need the goto from above
    out:

        // Owning key version: copy to arena
        if (store_key) { (*m)->key = key.Copy(a); }
        // Non-owning key version: only copy str, buf is still points to buf of key
        else { (*m)->key = key; }

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

    // Copy of operator[] for legacy reasons
    V* Insert(Str key, Arena* a, bool store_key = false)
    {
        Trie<V>*  buf[1] = {this};
        Trie<V>** m      = buf;
        for (auto h = key.Hash64(); *m; h <<= 2)
        {
            if (key == (*m)->key) { goto out; } // Found
            else { m = &(*m)->child[h >> 62]; } // Check child
        }

        if (!a) { return 0; } // No insert, so return not found

        *m = a->Make<Trie<V>>(); // Create new map

    // Expected behaviour is that if we call second time with store_key==true,
    // it should store it. Therefore, we need the goto from above
    out:

        // Owning key version: copy to arena
        if (store_key) { (*m)->key = key.Copy(a); }
        // Non-owning key version: only copy str, buf is still points to buf of key
        else { (*m)->key = key; }

        return &(*m)->val; // Val for set/get
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
