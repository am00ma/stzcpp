#pragma once

#include "slice.h"

// Hash
#define FNV_64_OFFSET_BASIS 0xcbf29ce484222325
#define FNV_64_PRIME        1099511628211

typedef struct Buf Buf;

/* ---------------------------------------------------------------------------
 * Str
 * ------------------------------------------------------------------------- */

typedef struct Str : List<char> {

    /* ---------------------------------------------------------------------------
     * Lifetime
     * ------------------------------------------------------------------------- */

    Str() = default;

    // From fields
    Str(char* buf_, isize len_)
    {
        buf = buf_;
        len = len_;
    }

    // From literals
    template <isize N> constexpr Str(char (&s)[N])
    {
        buf = s;
        len = N - 1;
    }

    // From const literals
    template <isize N> constexpr Str(const char (&s)[N])
    {
        cbuf = s;
        len  = N - 1;
    }

    // From List
    Str(List<char> l)
    {
        buf = l.buf;
        len = l.len;
    }

    // From Span
    Str(char* beg, char* end)
    {
        buf = beg;
        len = beg ? end - beg : 0;
    }

    // From arena (zeroed, defaults, non-zeroed as needed)
    Str(Arena* a, isize len_, b32 flags = 0)
    {
        buf = a->Make<char>(len_, flags);
        len = len_;
    };

    /* ---------------------------------------------------------------------------
     * Methods
     * ------------------------------------------------------------------------- */

    // Copy to arena
    Str Copy(Arena* a, bool null_terminate = false)
    {
        if (buf == a->beg - len)
        {
            if (null_terminate)
            {
                *a->beg = '\0';
                a->beg++;
            }
            return *this;
        }
        Str dst = Str(a, len + (isize)null_terminate); // zero initialized
        if (len) { memcpy(dst.buf, buf, len); }
        dst.len -= int(null_terminate);
        return dst;
    }

    // Null terminated string
    char* Cstr(Arena* a)
    {
        Str dst = this->Copy(a, true);
        return dst.buf;
    }

    // Takes care of len and bounds
    bool StartsWith(Str s)
    {
        if (len < s.len) { return false; }
        return (*this)[0, s.len] == s;
    };

    // Takes care of len and bounds
    bool EndsWith(Str s)
    {
        if (len < s.len) { return false; }
        return (*this)[len - s.len, len] == s;
    };

    // FNV hash
    u64 Hash64()
    {
        u64 h = FNV_64_OFFSET_BASIS;
        for (isize i = 0; i < len; i++)
        {
            h ^= buf[i] & 255;
            h *= FNV_64_PRIME;
        }
        return h;
    }

    // Split (defaults to splitting lines)
    List<Str> Split(Arena* a, Str delimiter = "\n", bool ignore_empty = true, bool substitute_null = false,
                    isize max_parts = 1024)
    {
        Assert(delimiter.len == 1);

        char*      start = &buf[0];
        Slice<Str> parts = Slice<Str>(a, max_parts);
        RANGE(i, len)
        {
            if (buf[i] == delimiter.buf[0])
            {
                isize pos = &buf[i] - start;
                if (pos || !ignore_empty) { parts += Str(start, pos); }
                if (substitute_null) buf[i] = '\0'; // Trick like strtok (buf is not const literal)
                start = &buf[i] + 1;                // Skip delimiter
            }
        }
        if ((isize)(start - buf) <= len)
        {
            isize pos = len - (start - buf);
            if (pos || !ignore_empty) { parts += Str(start, len - (start - buf)); }
        }
        parts.Shrink(a);
        return parts;
    };

    // Join (defaults to splitting lines)
    Str Join(Arena* a, List<Str> strings)
    {
        isize num = 0;
        RANGE(i, strings.len) { num += strings[i]->len; }
        num += (strings.len * len);

        Slice<char> parts = Slice<char>(a, num);
        RANGE(i, len)
        {
            parts += *strings[i];
            parts += *this;
        }
        Str out = parts.Shrink(a);
        return out;
    };

} Str;

/* ---------------------------------------------------------------------------
 * Buf - only constructors
 * ------------------------------------------------------------------------- */

typedef struct Buf : Slice<char> {

    Buf() = default;

    // Conversion from Str
    Buf(Str l)
    {
        buf = l.buf;
        len = l.len;
        cap = l.len;
    };

    // Conversion from Slice
    Buf(Slice<char> s)
    {
        buf = s.buf;
        len = s.len;
        cap = s.cap;
    };

    // Canonical usage: init from static buf
    template <isize N> constexpr Buf(char (&s)[N])
    {
        List<char>::buf = s;
        List<char>::len = 0;
        cap             = N;
    }

    // From arena (cap = cap_, len = 0; zeroed, defaults, non-zeroed)
    Buf(Arena* a, isize cap_, b32 flags = 0)
    {
        buf = a->Make<char>(cap_, flags);
        len = 0;
        cap = cap_;
    };

    // Extend slice, increment len
    // Should have pref over Slice<T>& operator+=(List<T> val)
    Buf& operator+=(Str val)
    {
        Assert(len + val.len <= cap);
        if (val.len) { memcpy(&buf[len], val.buf, val.len); }
        len += val.len;
        return *this;
    }

    // Reallocs capacity
    // Should have pref over Slice<T>& operator+=(List<T> val)
    Buf& Push(Arena* a, Str val)
    {
        if (len + val.len > cap)
        {
            isize oldcap = cap;
            bool  ontop  = a->beg == (char*)buf + cap;
            while (cap < (len + val.len)) cap *= 2;
            if (ontop) { Buf(a, cap - oldcap); }
            else
            {
                Buf dst = {a, cap}; // New underlying buffer
                memcpy(dst.buf, buf, len);
                buf = dst.buf;
            }
        }
        memcpy(&buf[len], val.buf, val.len);
        len += val.len;
        return *this;
    }

} Buf;
