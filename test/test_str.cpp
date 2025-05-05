#include "arena.h"
#include "range.h"
#include "str.h"
// #include "log.h"
// #include "range.h"

#include <cstdio> // printf

int main(void)
{
    // ------------------------------------
    Arena a = Arena(256); // 256 bytes
    a.Print("Initial");

    Str x = " hello hi   how are  you  ";
    printf("|%.*s|\n", pstr(x));

    Strs parts = {};

    title("\n ignore_empty = true");
    parts = x.Split(&a, " ");
    RANGE(i, parts.len) { printf("%ld: %.*s\n", i, pstr(parts.data[i])); }

    title("\n ignore_empty = false");
    parts = x.Split(&a, " ", false);
    RANGE(i, parts.len) { printf("%ld: %.*s\n", i, pstr(parts.data[i])); }

    return 0;
}
