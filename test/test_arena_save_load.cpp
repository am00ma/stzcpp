#include "arena.h"
#include "file.h"
#include "log.h"
#include "range.h"
#include "str.h"
#include <cassert>
#include <cstdio>

typedef struct Item {
    isize len = 0;
    i32*  a   = 0;
    i32*  b   = 0;

    Arena arena;

    // Lifetime
    Item(isize len_, Arena arena_)
    {
        len   = len_;
        arena = arena_;
        a     = arena.Make<i32>(len);
        b     = arena.Make<i32>(len);
    }

} Item;

int main(int argc, char* argv[])
{
    Arena perm = Arena(16 * 1024);
    perm.Print("init");

    Arena a1 = Arena(1024, &perm);
    Arena a2 = Arena(1024, &perm);

    a1.Print("a1: Before Item alloc");

    Item item1 = Item(5, a1);

    printf("Item: a, b: %p, %p\n", (void*)item1.a, (void*)item1.b);

    RANGE(i, item1.len)
    { //
        item1.a[i] = 5;
        item1.b[i] = 5;
    }

    a1.Print("a1: After Item alloc");

    Str   path      = "/tmp/item.bin";
    char* path_cstr = path.Cstr(&a1);
    FILE* f         = fopen(path_cstr, "wb");
    if (f == NULL)
    {
        perror("fopen");
        Fatal(-1, "fopen");
    }

    int ok = fwrite(&item1, sizeof(Item), 1, f);
    if (!ok)
    {
        perror("fwrite");
        Fatal(-1, "fwrite");
    }

    if (fclose(f) != 0)
    {
        perror("fclose");
        Fatal(-1, "fclose");
    }

    a2.Print("a2: Before file read");

    auto loaded_ok = File_Read(&a2, path);
    Fatal(loaded_ok.err, "File_Read");
    Str data = loaded_ok.data;

    a2.Print("a2: After file read");

    assert(data.len == sizeof(Item));
    printf("data.len    : %ld\n", data.len);
    printf("sizeof(Item): %ld\n", sizeof(Item));

    Item* item2 = (Item*)data.buf;

    printf("Item: a, b: %p, %p\n", (void*)item2->a, (void*)item2->b);
    item2->arena.Print("ll->arena(a1): After load");

    // Sanitize pointers
    isize offset_a = (char*)item2->a - item2->arena.OrigBeg();
    isize offset_b = (char*)item2->b - item2->arena.OrigBeg();
    printf("Offsets: a, b: %ld, %ld\n", offset_a, offset_b);

    item2->a = (i32*)(a2.OrigBeg() + offset_a);
    item2->b = (i32*)(a2.OrigBeg() + offset_b);
    printf("New pos: a, b: %p, %p\n", (void*)item2->a, (void*)item2->b);

    RANGE(i, item2->len)
    { //
        printf("%ld: %d, %d\n", i, item2->a[i], item2->b[i]);
    }

    return 0;
}
