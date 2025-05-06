#pragma once

#include "str.h"   // Str

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
