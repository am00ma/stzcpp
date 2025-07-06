#include "str.h"
#include "log.h"

#include <cassert> // assert
#include <cstdarg> // va_start, va_end, va_list
#include <cstring>

/* ---------------------------------------------------------------------------
 * Initialization
 * ------------------------------------------------------------------------- */
Strs::Strs(Arena* a, isize len_)
{
    data = a->Make<Str>(len_);
    len  = len_;
};

Str::Str(Arena* a, isize len_)
{
    buf = a->Make<char>(len_);
    len = len_;
}

Str::Str(const char* buf_)
{
    if (!buf_) { buf_ = ""; } // Empty string if invalid buf
    cbuf = buf_;
    len  = cbuf ? strlen(cbuf) : 0;
}

Str::Str(char* buf_, isize len_)
{
    buf = buf_;
    len = len_;
}

Str::Str(char* beg, char* end)
{
    assert(beg <= end);
    buf = beg;
    len = end - beg;
}

Str::Str(Arena* a, isize maxlen, char const* fmt, ...)
{
    char* beg = a->beg;
    buf       = a->Make<char>(maxlen);

    va_list arg;
    va_start(arg, fmt);
    len = vsnprintf(buf, maxlen, fmt, arg);
    va_end(arg);

    a->beg = beg + len; // Discard extra (not null terminated?)
};

/* ---------------------------------------------------------------------------
 * Operators
 * ------------------------------------------------------------------------- */
char& Str::operator[](isize i)
{
    assert(i >= 0);
    assert(i < len);
    return buf[i];
}

Str Str::operator[](isize beg, isize end)
{
    assert(beg >= 0);
    assert(beg <= end);
    assert(end <= len);
    return {buf + beg, end - beg};
}

bool Str::operator==(Str s) { return len == s.len && (!len || !memcmp(buf, s.buf, len)); }

/* ---------------------------------------------------------------------------
 * Memory, interface with C strings
 * ------------------------------------------------------------------------- */
Str Str::Copy(Arena* a, bool null_terminate)
{
    // If on top of arena, just advance arena for reserved 0 byte
    // NOTE: Not sure this makes sense
    if (buf == a->beg - len)
    {
        debug("Already on top");
        *a->beg = '\0'; // NOTE: Is this correct?
        a->beg++;

        // Not reflecting null-termination
        return *this;
    }

    Str dst = Str(a, len + int(null_terminate)); // zero initialized
    if (len) memcpy(dst.buf, buf, len);

    // NOTE: Should this be reflected here?
    dst.len -= int(null_terminate);

    return dst;
}

char* Str::Cstr(Arena* a)
{
    Str dst = this->Copy(a, true);
    return dst.buf;
}

/* ---------------------------------------------------------------------------
 * String manipulation (split, trim, etc.)
 * ------------------------------------------------------------------------- */
Strs Str::Split(Arena* a, Str delimiter, bool ignore_empty, bool substitute_null)
{
    // TODO: implement for len > 1
    assert(delimiter.len == 1);

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

    // Last line
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

/* ---------------------------------------------------------------------------
 * Hash
 * ------------------------------------------------------------------------- */
#define FNV_64_OFFSET_BASIS 0xcbf29ce484222325
#define FNV_64_PRIME        1099511628211

u64 Str::Hash64()
{
    u64 h = FNV_64_OFFSET_BASIS;
    for (isize i = 0; i < len; i++)
    {
        h ^= buf[i] & 255;
        h *= FNV_64_PRIME;
    }
    return h;
};
