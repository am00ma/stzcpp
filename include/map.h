#pragma once

#include "arena.h" // Arena
#include "str.h"   // Str
#include "types.h" // isize

/* ---------------------------------------------------------------------------
 * Since we use templates, we have to define in header file
 * ------------------------------------------------------------------------- */
template <typename V> struct Map {
    Str*  keys    = 0; // Keys are strings as we need method `u64 Hash64()` and operator `==`
    V*    vals    = 0; // Any type
    isize cap_exp = 4; // Default to 16 elements

    /* ---------------------------------------------------------------------------
     * Initialization
     * ------------------------------------------------------------------------- */
    Map(Arena* a)
    {
        keys = a->Make<Str>(1 << cap_exp); // Default = 4 (16 elements)
        vals = a->Make<V>(1 << cap_exp);
    };

    Map(Arena* a, isize cap_exp_)
    {
        cap_exp = cap_exp_;
        keys    = a->Make<Str>(1 << cap_exp); // Default = 4 (16 elements)
        vals    = a->Make<V>(1 << cap_exp);
    };

    /* ---------------------------------------------------------------------------
     * Lookup and Insert
     * ------------------------------------------------------------------------- */
    // Now we leverage equals and hash64 for a double-hashed, open address search on
    // the keys array.
    // By returning a pointer to the unmodified value slot, this function covers
    // both lookup and insertion. So that’s the entire hash table implementation. To
    // insert, the caller assigns the slot. For mere lookup, check the slot for a
    // null pointer.
    V* Lookup(Str key)
    {
        u64 hash = key.Hash64(); // Hashing: O(n)
        u64 mask = (1 << cap_exp) - 1;
        u64 step = (hash >> (64 - cap_exp)) | 1;
        for (i64 i = hash;;)
        {
            i = (i + step) & mask;
            if (!keys[i].buf)
            {
                keys[i] = key;
                return vals + i;
            }
            else if (keys[i] == key) { return vals + i; } // Equals: O(n)
        }
        return 0;
    };
};
