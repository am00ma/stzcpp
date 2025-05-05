#include "arena.h"

#include <cstdio> // printf

int main(void)
{
    Arena a = Arena(16 * 1024);

    a.Print("Initial");

    int* x = a.New<int>();
    x[0]   = 0;
    printf("%d: %d\n", 0, x[0]);
    a.Print("Alloc: 1 int");

    int* y = a.New<int>(10);
    for (int i = 0; i < 10; i++) { y[i] = i; }

    for (int i = 0; i < 10; i++) { printf("%d: %d\n", i, x[i]); }
    a.Print("Alloc: 10 int");

    return 0;
}
