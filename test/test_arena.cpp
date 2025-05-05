#include "arena.h"
#include "log.h"
#include "range.h"

#include <cstdio> // printf

typedef struct Item {
    i32 a = 4;
    i32 b = 8;
} Item;

int main(void)
{
    // ------------------------------------
    Arena a = Arena(128); // 128 bytes
    a.Print("Initial");

    // ------------------------------------
    title("\nSingle element");

    i32* x = a.Make<i32>();

    printf("%d: %d\n", 0, x[0]);
    a.Print("Alloc: 1 i32");

    // ------------------------------------
    title("\nArray of elements (zeroed by default)");

    i32* y = a.Make<i32>(3);

    RANGE(i, 3) { printf("%ld: %d\n", i, y[i]); }
    a.Print("Alloc: 3 i32");

    // ------------------------------------
    title("\nElements zeroed");

    Item* qz = a.Make<Item>(3); // Have to get order of new correct

    RANGE(i, 3) { printf("%ld: %d, %d\n", i, qz[i].a, qz[i].b); }
    a.Print("Zeroed: 3 Items");

    // ------------------------------------
    title("\nElements with defaults");

    Item* qd = a.Make<Item>(3, DEFAULTS); // Have to get order of new correct

    RANGE(i, 3) { printf("%ld: %d, %d\n", i, qd[i].a, qd[i].b); }
    a.Print("Defaults: 3 Items");

    // ------------------------------------
    title("\nElements with default args");

    Item* qa = a.Make<Item>(3, DEFAULTS, 3, 5); // Have to get order of new correct

    RANGE(i, 3) { printf("%ld: %d, %d\n", i, qa[i].a, qa[i].b); }
    a.Print("Args: 3 Items");

    // ------------------------------------
    return 0;
}
