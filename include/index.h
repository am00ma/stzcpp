#pragma once

#include "arena.h" // Arena
#include "log.h"   // error
#include "range.h" // RANGE
#include "types.h" // isize

template <typename K, typename V> struct Index {
    K*    keys = 0;  // Any type, must support `==`
    V*    vals = 0;  // Any type
    isize len  = 0;  // Current length
    isize cap  = 16; // Default to 16 elements

    /* ---------------------------------------------------------------------------
     * Initialization
     * ------------------------------------------------------------------------- */
    Index(Arena* a)
    {
        keys = a->Make<K>(cap); // Default = 4 (16 elements)
        vals = a->Make<V>(cap);
    };

    Index(Arena* a, isize cap_)
    {
        cap  = cap_;
        keys = a->Make<K>(cap); // Default = 4 (16 elements)
        vals = a->Make<V>(cap);
    };

    /* ---------------------------------------------------------------------------
     * Lookup and Insert
     * ------------------------------------------------------------------------- */
    V* Lookup(K key)
    {
        // Search list
        RANGE(i, len)
        {
            if (key == keys[i]) { return &vals[i]; }
        }

        // Was not found
        if (len >= cap)
        {
            error("Out of memory");
            return 0;
        }

        // Append to index
        len++;
        keys[len - 1] = key;
        return &vals[len - 1];
    };
};
