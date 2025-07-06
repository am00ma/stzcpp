#include "arena.h"
#include "file.h"
#include "log.h"

#include <cstdio> // printf

int main(void)
{
    // ------------------------------------
    Arena a = Arena(16 * 1024); // 16KB
    a.Print("Initial");

    title("File not found");
    auto res1 = File_Read(&a, "./hello");
    if (res1.err) { printf("Error: %d\n", res1.err); }
    else { printf("%.*s", pstr(res1.data)); }
    a.Print("After failed read");

    title("File found");
    auto res2 = File_Read(&a, "./README.md");
    if (res2.err) { printf("Error: %d\n", res2.err); }
    else { printf("%.*s", pstr(res2.data)); }
    a.Print("Final");

    return 0;
}
