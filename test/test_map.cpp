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
    Arena perm = Arena(1024);

    Map map = Map<Str>(&perm); // 2^4 = 16 elements when full

    // Insert some values
    *map.Lookup("hello") = "hi";  // Store "hi" as value for key "hello"
    *map.Lookup("how")   = "are"; //
    *map.Lookup("you")   = "";    // Can store empty strings

    // Retrieve values
    map_print(map, "hello");
    map_print(map, "how");
    map_print(map, "you");
    map_print(map, "doing");

    return 0;
}
