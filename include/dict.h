#pragma once

#include "str.h"

template <typename K, typename V> struct DictChild {

    DictChild<K, V>* child[4] = {};

    Str key;
    V   val;

    void Print() { printf("%.*s -> %.*s \n", pstr(key), pstr(val)); }

    void PrintChildren()
    {
        printf("   0: %p \n", (void*)child[0]);
        printf("   1: %p \n", (void*)child[1]);
        printf("   2: %p \n", (void*)child[2]);
        printf("   3: %p \n", (void*)child[3]);
    }
};

template <typename K, typename V> struct Dict {

    DictChild<K, V>        root = {};
    Slice<DictChild<K, V>> data = {};

    /* ---------------------------------------------------------------------------
     * Initialization - Usual zero init
     * ------------------------------------------------------------------------- */

    Dict() = default; // Usual zero-initialization

    // From Arena
    Dict(Arena* a, isize cap_) { data = Slice<DictChild<K, V>>(a, cap_); }

    /* ---------------------------------------------------------------------------
     * Lookup and Insert
     * ------------------------------------------------------------------------- */

    V* operator[](K key, bool insert = true, Arena* a = 0)
    {
        DictChild<K, V>*  buf[1] = {&root};
        DictChild<K, V>** m      = buf;

        for (auto h = key.Hash64(); *m; h <<= 2)
        {
            // Found
            if (key == (*m)->key)
            {
                (*m)->key = key;
                return &(*m)->val; // Val for set/get
            }
            else { m = &(*m)->child[h >> 62]; } // Check child
        }

        // No insert, so return not found (len does not increment)
        if (!insert) { return 0; }

        // We have a new key -> no allocation, just assign
        if (!(data.len < data.cap))
        {
            if (!a) Assert(false);
            data.Reserve(a, data.cap * 2);
        }

        *m = &data.buf[data.len];
        data.len++;

        (*m)->key = key;
        return &(*m)->val; // Val for set/get
    }

    void Print(const char* label, bool children = false)
    {
        printf("%s: len: %ld, cap: %ld, data: %p\n", label, data.len, data.cap, (void*)data);
        RANGE(i, data.len)
        {
            printf("%ld: ", i);
            data[i].Print(); // Print children
            if (children) { data[i].PrintChildren(); }
        }
    }
};
