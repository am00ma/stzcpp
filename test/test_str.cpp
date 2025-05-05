#include "arena.h"
#include "log.h"
#include "range.h"
#include "str.h"

#include <cstdio> // printf

int main(void)
{
    // ------------------------------------
    Arena a = Arena(1024); // 1KB
    a.Print("Initial");

    Str x = " hello hi   how are  you  ";
    printf("|%.*s|\n", pstr(x));

    // ------------------------------------
    Strs parts = {};

    title("\n Split: ignore_empty = true");
    parts = x.Split(&a, " ");
    a.Print("After split (ignore)");
    RANGE(i, parts.len) { printf("%ld: %.*s\n", i, pstr(parts.data[i])); }

    title("\n Split: ignore_empty = false");
    parts = x.Split(&a, " ", false);
    RANGE(i, parts.len) { printf("%ld: %.*s\n", i, pstr(parts.data[i])); }
    a.Print("After split");

    title("\n Format");
    Str f = Str(&a, 256, "Number: %d", 10);
    printf("%.*s\n", pstr(f));
    a.Print("After format");

    title("\n Cstr");
    char* c = f.Cstr(&a);
    printf("%s\n", c);
    a.Print("After cstr");

    title("\n Cstr (triggers copy)");
    char* c2 = f.Cstr(&a);
    printf("%s\n", c2);
    a.Print("After second cstr");

    return 0;
}
