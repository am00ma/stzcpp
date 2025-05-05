#include "arena.h"
#include "str.h"
// #include "log.h"
// #include "range.h"

#include <cstdio> // printf

int main(void)
{
    // ------------------------------------
    Arena a = Arena(128); // 128 bytes
    a.Print("Initial");

    Str x = "hello";

    printf("%.*s\n", pstr(x));
}
