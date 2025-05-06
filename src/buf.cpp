#include "buf.h"
#include "log.h"

static inline void oom(isize len, isize clen, isize cap)
{
    error("Out of memory:\n"       //
          "  len + c.len >= cap\n" //
          "  %ld + %ld >= %ld",
          len, clen, cap);
    exit(-1);
}

Buf::Buf(Arena* a, isize cap_)
{
    buf = a->Make<char>(cap_);
    len = 0;
    cap = cap_;
}

Str Buf::Final(Arena* a)
{
    a->beg -= cap - len;
    cap     = len;
    return Str(buf, len);
}

Str Buf::Join(Str c)
{
    if (len + c.len >= cap) oom(len, c.len, cap); // Straight out oom, can add flags later
    memcpy(&buf[len], c.buf, c.len);
    len += c.len;
    return Str(buf, len);
};
