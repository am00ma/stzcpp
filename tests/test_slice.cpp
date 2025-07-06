#include "log.h"
#include "range.h"
#include "slice.h"

#include <cstdio> // printf

typedef struct Item {
    i32 a = 4;
    i32 b = 8;
} Item;

int main(void)
{
    // ------------------------------------
    Arena a = Arena(1024 * 1024); // 1 MB
    a.Print("Initial");

    // ------------------------------------
    title("Append");
    Slice<i32> s = Slice<i32>(&a, 2);

    s.Append(12);
    s.Append(24);
    RANGE(i, s.len) { debug("%ld: %d", i, s.data[i]); }

    // ------------------------------------
    title("Index");

    Slice<i32> vec = Slice<i32>(&a, 3);
    RANGE(i, vec.cap) { vec.Append(i); }
    RANGE(i, vec.len)
    {
        *vec[i] = i + 10;
        debug("%ld: %d", i, *vec[i]);
    }

    // ------------------------------------
    title("Subslice");

    Slice<i32> vec2 = Slice<i32>(&a, 10);
    RANGE(i, vec2.cap) { vec2.Append(i); }

    debug("Changing element of original");
    auto sub = vec2[5, 8];
    *sub[1] = 53;
    RANGE(i, sub.len) { debug("%ld: %d", i, *sub[i]); }

    debug("Check effect on origial");
    RANGE(i, vec2.len) { debug("%ld: %d", i, *vec2[i]); }

    // ------------------------------------
    title("Subslice - copy");

    Slice<i32> vec3 = Slice<i32>(&a, 10);
    RANGE(i, vec3.cap) { vec3.Append(i); }

    debug("Changing element of copy");
    auto sub2 = vec3[5, 8, &a];
    *sub2[1] = 53;
    RANGE(i, sub2.len) { debug("%ld: %d", i, *sub2[i]); }

    debug("Check effect on origial");
    RANGE(i, vec3.len) { debug("%ld: %d", i, *vec3[i]); }
}
