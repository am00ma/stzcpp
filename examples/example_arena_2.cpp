/*
 * Usual case, we want to allocate objects on heap inside function and
 * return a valid reference to the outer scope
 *
 * Is this true?
 *   As we cannot access the stack of the function, it HAS to either return a
 *   reference on the heap, or return the struct by copy
 *
 * Typical function that needs to return valid ref:
 *
 *      // Definition
 *      type type_new(..., Arena *a);
 *
 *      // Call
 *      type x = type_new(..., a);
 *      assert (x); // Not null
 *
 */

#include <cassert>
#include <stz/arena.h>

typedef struct Str {
    char* buf;
    isize len;

    Str(isize len, Arena* a)
    {
        // Set some wierd defaults
        buf = a->Make<char>(len);
        RANGE(i, len) { buf[i] = 65 + i; }
    }

} Str;

typedef struct Floats {
    f32*  buf;
    isize len;

    Floats(isize len, Arena* a)
    {
        // Set some wierd defaults
        buf = a->Make<f32>(len);
        RANGE(i, len) { buf[i] = 65.0 / i; }
    }

    f32& operator[](isize i)
    {
        assert(i >= 0);
        assert(i < len);
        return buf[i];
    }

} Floats;

int main()
{
    // Allocate memory (16 bytes)
    Arena a = Arena(1024);

    // Length of list
    i32 len = 10;

    // Create string
    auto word = Str(len, &a);
    printf("%.*s\n", (int)word.len, word.buf);

    // Create vector of floats
    auto range = Floats(len, &a);
    RANGE(i, len) { printf("%ld: %2.2f  ", i, range[i]); }
    printf("\n");

    return 0;
}
