#pragma once
/*
 * Str:
 *
 *   Fields:
 *
 *      union {
 *          char*       buf = 0;
 *          const char* cbuf;
 *      };
 *      isize len = 0;
 *
 *   Lifetime:
 *
 *      Str();
 *
 *      template <isize N> constexpr Str(const char (&s)[N]);  // From string literals
 *
 *      Str(const char* buf_);                     // From const char *
 *      Str(char* buf_, isize len_);               // From buf and len
 *      Str(Arena* a, isize len_, b32 flags = 0);  // Allocate from arena
 *      Str(char* beg_, char* end_);               // From span
 *
 *      Str(Arena* a, isize maxlen, char const* fmt, ...); // Formatted string on arena
 *
 *   Operators:
 *
 *      char& operator[](isize i);              // ith char by ref
 *      Str   operator[](isize beg, isize end); // Substring
 *      bool  operator==(Str   s);              // Equality
 *
 *   Methods:
 *
 *      char* Cstr(Arena* a);                            // C style null-terminated string
 *      Str Copy(Arena* a, bool null_terminate = false); // Ownership by copying to arena
 *
 *      bool StartsWith(Str s);
 *      bool EndsWith(Str s);
 *      u64  Hash64()
 *
 *      Slice<Str> Split(
 *          Arena* a,
 *          Str    delimiter       = "\n",
 *          bool   ignore_empty    = true,
 *          bool   substitute_null = false,
 *          isize  max_parts       = 1024
 *      );
 *
 *   Debugging:
 *
 *      void Print(const char* label);
 *
 *   Helpers:
 *
 *      typedef Slice<Str> Strs;
 *
 * Notes:
 *
 *  1. `Slice(T* buf, isize len_);` is convinience to set len equal to cap on init
 *  2. `Slice<T> Final(Arena* a);` is tricky but very useful
 *  3. `cap` is needed only for debugging and `Free`
 *  4. `Strs` is commonly used so typedefd here
 *
 * */

#include "arena.h"
#include "slice.h"
#include <cstdarg> // va_start, va_end, va_list

/* ---------------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------------- */

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

    // Split (defaults to splitting lines)
    Slice<Str> Split(Arena* a, Str delimiter = "\n", bool ignore_empty = true, bool substitute_null = false,
                     isize max_parts = 1024)
    {
        // TODO: implement for len > 1
        Assert(delimiter.len == 1);

        // Start position
        char* start = &buf[0];

        // Alloc dynamically (no other user/variable on arena)
        Slice<Str> parts = Slice<Str>(a, max_parts);
        for (int i = 0; i < len; i++)
        {
            if (buf[i] == delimiter.buf[0])
            {
                isize pos = &buf[i] - start;
                if (pos || !ignore_empty) { parts + Str(start, pos); }

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
            if (pos || !ignore_empty) { parts + Str(start, len - (start - buf)); }
        }

        // Reclaim space
        parts.Final(a);

        return parts;
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

    // For debugging
    void Print(const char* label)
    {
        printf("%s: %.*s\n  len: %ld\n  buf: %p\n", //
               label, (int)len, buf, len, (void*)buf);
    }

} Str;

/* ---------------------------------------------------------------------------
 * Special case of Slice of Strs
 * ------------------------------------------------------------------------- */
typedef Slice<Str> Strs;
