#pragma once

#include "log.h" // Fatal, Assert
#include "str.h" // Str

typedef struct Buf {

    char* buf = 0;
    isize len = 0;
    isize cap = 0;

    Buf() = default; // Zero-init?

    // Initialization
    Buf(Arena* a, isize cap_)
    {
        buf = a->Make<char>(cap_);
        len = 0;
        cap = cap_;
    }

    // Shrinks arena
    // NOTE: provided no new objects after declaration of Buf
    Str Final(Arena* a)
    {
        a->beg -= cap - len;
        cap     = len;
        return Str(buf, len);
    }

    // Use with '+', Exactly same as Join
    Buf* operator+(Str c)
    {
        if (len + c.len >= cap)
        {
            Fatal(-1, "Buf: len + c.len >= cap : %ld + %ld ( = %ld ) >= %ld", //
                  len, c.len, len + c.len, cap);
        }
        memcpy(&buf[len], c.buf, c.len);
        len += c.len;
        return this; // By reference -> clears up mysteries around chaining
    }

    // Get ith item by reference
    char* operator[](isize i)
    {
        Assert(i >= 0);
        Assert(i < len);
        return &buf[i];
    }

    // Get Str representation
    Str operator[]() { return Str(buf, len); }

    // Get (i - j)th item by reference
    Str operator[](isize i, isize j)
    {
        Assert(i >= 0);
        Assert(j >= i);
        Assert(j < len);
        return Str(&buf[i], j - i);
    }

    // Get (i - j)th item by value
    Str operator[](isize i, isize j, Arena* a)
    {
        Assert(i >= 0);
        Assert(j >= i);
        Assert(j < len);
        return Str(&buf[i], j - i).Copy(a);
    }

    // Identical to + operator
    Str Join(Str c)
    {
        if (len + c.len >= cap)
        {
            Fatal(-1, "Buf: len + c.len >= cap : %ld + %ld ( = %ld ) >= %ld", //
                  len, c.len, len + c.len, cap);
        }
        memcpy(&buf[len], c.buf, c.len);
        len += c.len;
        return Str(buf, len);
    };

} Buf;
