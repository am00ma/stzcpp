#include "log.h"   // title
#include "range.h" // RANGE
#include <cstdio>  // printf

int main()
{

    title("Single arg: RANGE(i): `for(isize i = 0; i < 1; i++)`");
    RANGE(i) { printf("i: %ld\n", i); }

    title("i can be reused");
    RANGE(i) { printf("i: %ld\n", i); }

    title("j if you need");
    RANGE(j) { printf("j: %ld\n", j); }

    title("Two args: RANGE(i, n): `for(isize i = 0; i < n; i++)`");
    RANGE(i, 10) { printf("i: %ld\n", i); }

    title("Nested loops");
    RANGE(i, 3) RANGE(j, 3) { printf("i, j: %ld, %ld\n", i, j); }

    title("Three args: RANGE(i, a, b): `for(isize i = a; i < b; i++)`");
    RANGE(i, 2, 3) { printf("i: %ld\n", i); }

    title("Four args: RANGE(i, a, b, c): `for(isize i = a; i < b; i+= c)`");
    RANGE(i, 5, 10, 2) { printf("i: %ld\n", i); }

    title("Scope: Without `{ ... }`");
    RANGE(i, 3) printf("i: %ld\n", i);

    title("Scope: With `{ ... }`");
    RANGE(i, 3) { printf("i: %ld\n", i); }

    return 0;
}
