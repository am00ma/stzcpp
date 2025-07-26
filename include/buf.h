#pragma once

#include "log.h" // Fatal
#include "str.h" // Str
#include <cassert>

typedef struct Buf {
    char* buf;
    isize len;
    isize cap;

    // Initialization
    Buf(Arena* a, isize cap_)
    {
        buf = a->Make<char>(cap_);
        len = 0;
        cap = cap_;
    }

    // API for interaction, usage

    // Returns Str, modifies self
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

    // Shrinks arena
    // NOTE: provided no new objects after declaration of Buf
    Str Final(Arena* a)
    {
        a->beg -= cap - len;
        cap     = len;
        return Str(buf, len);
    }

    // Use with '+', Exactly same as Join
    Str operator+(Str c)
    {
        if (len + c.len >= cap)
        {
            Fatal(-1, "Buf: len + c.len >= cap : %ld + %ld ( = %ld ) >= %ld", //
                  len, c.len, len + c.len, cap);
        }
        memcpy(&buf[len], c.buf, c.len);
        len += c.len;
        return Str(buf, len);
    }

    // Get ith item by reference
    char* operator[](isize i)
    {
        assert(i >= 0);
        assert(i < len);
        return &buf[i];
    }

    // Get Str representation
    Str operator[]() { return Str(buf, len); }

    // Get (i - j)th item by reference
    Str operator[](isize i, isize j)
    {
        assert(i >= 0);
        assert(j >= i);
        assert(j < len);
        return Str(&buf[i], j - i);
    }

    // Get (i - j)th item by value
    Str operator[](isize i, isize j, Arena* a)
    {
        assert(i >= 0);
        assert(j >= i);
        assert(j < len);
        return Str(&buf[i], j - i).Copy(a);
    }

} Buf;
