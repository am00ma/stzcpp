#include <stz/arena.h>

char* new_hello(Arena* a)
{
    // Some internal state
    const char* internal = "hello";

    // Allocate memory
    char* out = a->Make<char>(strlen(internal) + 1);

    // Copy to internal state output / Modify output
    out = (char*)"hello";

    // Return valid pointer
    return out;
}

int main()
{
    // Allocate memory (16 bytes)
    Arena a = Arena(16);

    // Call function with ref to arena as arg
    char* x = new_hello(&a);

    // Use output
    printf("%s\n", x);

    return 0;
}
