#include "arena.h"
#include "map.h"
#include <cstdio>

void map_print(Map<Str> map, Str key)
{
    // Lookup when called with 0 as lhs
    Str* val = map.Lookup(key);

    // Check val->buf for valid Str
    if (val->buf) printf("%.*s : %.*s\n", pstr(key), ppstr(val));
    else printf("Not found: %.*s\n", pstr(key));
}

int main(void)
{
    // Allocate on arena
    isize cap_exp = 4;
    Arena perm    = Arena((1 << cap_exp) * 2 * sizeof(Str)); // Exact memory required
    Map   map     = Map<Str>(&perm, cap_exp);                // 2^4 = 16 elements when full

    // Insert some values
    *map.Lookup("hello") = "hi";
    map_print(map, "hello");

    *map.Lookup("how") = "are";
    map_print(map, "how");

    // Can store empty strings
    *map.Lookup("you") = "";
    map_print(map, "you");

    // Check missing key
    map_print(map, "doing");

    // Delete entry
    //  - is this correct??
    //  - what happens to collisions?
    //  - if datatype is `int`?
    //  - don't we need gravestones?
    *map.Lookup("hello") = Str{};
    map_print(map, "hello");

    return 0;
}
