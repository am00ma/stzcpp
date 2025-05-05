#include "arena.h"
#include "range.h"

#include <cstdio> // printf

typedef struct Item {
    i32 a = 4;
    i32 b = 8;

} Item;

int main(void)
{
    Arena a = Arena(1024);

    a.Print("Initial");

    i32* x = a.Make<i32>();
    x[0]   = 0;
    printf("%d: %d\n", 0, x[0]);
    a.Print("Alloc: 1 i32");

    RANGE(j, 10)
    {
        i32* z = a.Make<i32>(10, NOZERO);
        RANGE(i, 10) { printf("%d ", z[i]); }
        printf("\n");
    }

    i32* y = a.Make<i32>(10);
    RANGE(i, 10) { y[i] = i; }
    RANGE(i, 10) { printf("%ld: %d\n", i, y[i]); }
    a.Print("Alloc: 10 i32");

    Item* q = a.Make<Item>(10, DEFAULTS);
    RANGE(i, 10) { printf("%ld: %d, %d\n", i, q[i].a, q[i].b); }
    a.Print("Alloc: 10 Items");

    return 0;
}
