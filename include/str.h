#pragma once

#include "arena.h" // Arena
#include <cstdarg> // va_start, va_end, va_list

/* ---------------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------------- */

// Fron stackoverflow, could not figure out how to use
template <typename T, int sz> int size(T (&)[sz]) { return sz; }

// Used in each other's declarations
typedef struct Str  Str;
typedef struct Strs Strs;

// For printing with `%.*s`
#define pstr(x)  (int)x.len, x.buf
#define ppstr(x) (int)x->len, x->buf

// Hash
#define FNV_64_OFFSET_BASIS 0xcbf29ce484222325
#define FNV_64_PRIME        1099511628211

/* ---------------------------------------------------------------------------
 * Counted string
 * ------------------------------------------------------------------------- */
typedef struct Str {
    // Hack for holding string literals as well
    union {
        char*       buf = 0;
        const char* cbuf;
    };
    isize len = 0;

    /* ---------------------------------------------------------------------------
     * Constructors
     * ------------------------------------------------------------------------- */

    // Default constructor -> zero by default
    Str() = default;

    // From string literals
    template <isize N> constexpr Str(const char (&s)[N])
    {
        cbuf = s;
        len  = N - 1;
    }

    // From const
    Str(const char* buf_)
    {
        if (!buf_) { buf_ = ""; } // Empty string if invalid buf
        cbuf = buf_;
        len  = cbuf ? strlen(cbuf) : 0;
    }

    // From fields
    Str(char* buf_, isize len_)
    {
        buf = buf_;
        len = len_;
    }

    // From Arena
    Str(Arena* a, isize len_, b32 flags = 0)
    {
        buf = a->Make<char>(len_, flags);
        len = len_;
    }

    // From spans
    Str(char* beg_, char* end_)
    {
        buf = beg_;
        len = end_ - beg_;
    }

    // From formatted string
    Str(Arena* a, isize maxlen, char const* fmt, ...)
    {
        char* beg = a->beg;
        buf       = a->Make<char>(maxlen);

        va_list arg;
        va_start(arg, fmt);
        len = vsnprintf(buf, maxlen, fmt, arg);
        va_end(arg);

        a->beg = beg + len; // Discard extra (not null terminated?)
    }

    /* ---------------------------------------------------------------------------
     * Operators
     * ------------------------------------------------------------------------- */

    // Get ith char
    char& operator[](isize i)
    {
        Assert(i >= 0);
        Assert(i < len);
        return buf[i];
    }

    // Get slice
    Str operator[](isize beg, isize end)
    {
        Assert(beg >= 0);
        Assert(beg <= end);
        Assert(end <= len);
        return {buf + beg, end - beg};
    }

    // Equality
    bool operator==(Str s) { return len == s.len && (!len || !memcmp(buf, s.buf, len)); }

    /* ---------------------------------------------------------------------------
     * Memory, interface with C strings
     * ------------------------------------------------------------------------- */

    // Null terminated string
    char* Cstr(Arena* a)
    {
        Str dst = this->Copy(a, true);
        return dst.buf;
    }

    // Copy to arena
    Str Copy(Arena* a, bool null_terminate = false)
    {
        // If on top of arena, just advance arena for reserved 0 byte
        // NOTE: Not sure this makes sense
        if (buf == a->beg - len)
        {
            if (null_terminate)
            {
                // // BUG: Something is messed up here, why are we null terminating?
                *a->beg = '\0'; // NOTE: Is this correct?
                a->beg++;
            }

            // Else just return
            return *this;
        }

        Str dst = Str(a, len + int(null_terminate)); // zero initialized
        if (len)
        {
            // debug("[M] Copied");
            memcpy(dst.buf, buf, len);
        }

        // NOTE: Should this be reflected here?
        dst.len -= int(null_terminate);

        return dst;
    }

    // Split (defaults to splitting lines)
    Strs Split(Arena* a, Str delimiter = "\n", bool ignore_empty = true, bool substitute_null = false);

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

    // For debugging
    void Print(const char* label)
    {
        printf("%s: %.*s\n  len: %ld\n  buf: %p\n", //
               label, (int)len, buf, len, (void*)buf);
    }

} Str;

/* ---------------------------------------------------------------------------
 * List of strings: Strs - Slice<Str> without cap -> is this really needed? Why data and not buf?
 * ------------------------------------------------------------------------- */

typedef struct Strs {

    Str*  data = 0; // Usual zero initialization
    isize len  = 0;

    Strs() = default;

    Strs(Arena* a, isize len_)
    {
        data = a->Make<Str>(len_);
        len  = len_;
    }

    Strs(Str* data_, isize len_)
    {
        data = data_;
        len  = len_;
    }

    // Get ith item by reference
    Str* operator[](isize i)
    {
        if (i < 0) return 0;
        return &data[i];
    };

    // Get (i - j)th item by reference
    Strs operator[](isize i, isize j)
    {
        Assert(i >= 0);
        Assert(j >= i);
        Assert(j < len);
        return Strs(&data[i], j - i);
    }

    // Get (i - j)th item as copy
    Strs operator[](isize i, isize j, Arena* a)
    {
        Assert(i >= 0);
        Assert(j >= i);
        Assert(j < len);
        auto slice = Strs(a->Make<Str>(j - i), j - i);
        if (slice.len) memcpy(slice.data, &data[i], slice.len * sizeof(Str));
        return slice;
    }

    // Eqality - actually compare strings, order
    bool operator==(Strs s)
    {
        if (len != s.len) { return false; }
        RANGE(i, len)
        {
            if (data[i] != *s[i]) { return false; }
        }
        return true;
    }

} Strs;

/* ---------------------------------------------------------------------------
 * Outliers
 * ------------------------------------------------------------------------- */

// BUG: Needs `inline` -> how does that change anything about definitions?
// Needs to be defined after Strs
inline Strs Str::Split(Arena* a, Str delimiter, bool ignore_empty, bool substitute_null)
{
    // TODO: implement for len > 1
    Assert(delimiter.len == 1);

    // Start position
    char* start = &buf[0];

    // Alloc dynamically (no other user/variable on arena)
    Strs parts = Strs(a, 0);
    for (int i = 0; i < len; i++)
    {
        if (buf[i] == delimiter.buf[0])
        {
            isize pos = &buf[i] - start;
            if (pos || !ignore_empty)
            {
                a->Make<Str>();
                parts.data[parts.len] = Str(start, pos);
                parts.len++;
            }

            // Trick like strtok to get char**
            if (substitute_null) buf[i] = '\0';

            // Skip delimiter
            start = &buf[i] + 1;
        }
    }

    // Last part, if any remaining
    if ((isize)(start - buf) <= len)
    {
        isize pos = len - (start - buf);
        if (pos || !ignore_empty)
        {
            a->Make<Str>();
            parts.data[parts.len] = Str(start, len - (start - buf));
            parts.len++;
        }
    }

    return parts;
}
