#pragma once

#include "arena.h" // Arena
#include "log.h"   // error
#include "str.h"   // Str
#include "types.h" // isize

/* ---------------------------------------------------------------------------
 * OOM
 * ------------------------------------------------------------------------- */
static inline void oom(isize len, isize clen, isize cap)
{
    error("Out of memory:\n"       //
          "  len + c.len >= cap\n" //
          "  %ld + %ld >= %ld",
          len, clen, cap);
    exit(-1);
}

/* ---------------------------------------------------------------------------
 * Buffer
 * ------------------------------------------------------------------------- */
typedef struct Buf {
    char* buf;
    isize len;
    isize cap;

    Buf(Arena* a, isize cap_);

    Str Join(Str c);
    Str Final(Arena* a); // Shrinks arena
} Buf;
