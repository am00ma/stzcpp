#include "arena.h"
#include "index.h"
#include "str.h"
#include <cstdio>

void index_print(Index<Str, Str> index, Str key)
{
    // Lookup when called with 0 as lhs
    Str* val = index.Lookup(key);

    // Check val->buf for valid Str
    if (val->buf) printf("%.*s : %.*s\n", pstr(key), ppstr(val));
    else printf("Not found: %.*s\n", pstr(key));
}

int main(void)
{
    // Allocate on arena
    isize cap   = 16;
    Arena perm  = Arena(cap * 2 * sizeof(Str)); // Exact memory required
    Index index = Index<Str, Str>(&perm, cap);  // 2^4 = 16 elements when full

    // Insert some values
    *index.Lookup("hello") = "hi";
    index_print(index, "hello");

    *index.Lookup("how") = "are";
    index_print(index, "how");

    // Can store empty strings
    *index.Lookup("you") = "";
    index_print(index, "you");

    // Check missing key
    index_print(index, "doing");

    // Delete entry
    //  - is this correct??
    //  - what happens to collisions?
    //  - if datatype is `int`?
    //  - don't we need gravestones?
    *index.Lookup("hello") = Str{};
    index_print(index, "hello");

    return 0;
}
