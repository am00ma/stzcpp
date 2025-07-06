#include "arena.h"
#include "file.h"
#include "log.h"
#include "range.h"

typedef struct Item {
    i32*  a   = 0;
    isize len = 3;
    char* beg = 0;

    Item(Arena* arena)
    {
        beg = arena->beg;

        len = 3;
        a   = arena->Make<i32>(len);
    }
} Item;

int main(int argc, char* argv[])
{
    isize len = 3;

    Arena perm = Arena(16 * 1024);
    perm.Print(">> init");

    Item i1 = Item(&perm);
    Item i2 = Item(&perm);
    RANGE(i, len)
    {
        i1.a[i] = 5;
        i2.a[i] = 10;
    }

    title("before");
    printf("  a: %p, %p\n", (void*)i1.a, (void*)i2.a);
    printf("beg: %p, %p\n", (void*)i1.beg, (void*)i2.beg);
    RANGE(i, len) { printf("%td: %d, %d\n", i, i1.a[i], i2.a[i]); }

    // 'Transferring' i1 to i2 / 'Reanchor'
    i2.a = (i32*)((char*)i1.a + (i2.beg - i1.beg));

    title("after");
    printf("  a: %p, %p\n", (void*)i1.a, (void*)i2.a);
    printf("beg: %p, %p\n", (void*)i1.beg, (void*)i2.beg);
    RANGE(i, len) { printf("%td: %d, %d\n", i, i1.a[i], i2.a[i]); }

    title("loaded vs saved");

    auto saved_ok = Struct_Write(&perm, "/tmp/item.bin", &i1);
    if (saved_ok.err) Fatal(saved_ok.err, "Struct_Write");
    auto saved = saved_ok.data;
    RANGE(i, len) { printf(" Saved: %td: %d\n", i, saved->a[i]); }

    // Fuck up the loading due to refs
    perm.Free();
    RANGE(i, 5) { Arena(16 * 1024); }

    Arena perm2 = Arena(16 * 1024);
    perm2.Print(">> new");

    auto loaded_ok = Struct_Read<Item>(&perm2, "/tmp/item.bin");
    if (loaded_ok.err) Fatal(loaded_ok.err, "Struct_Read");
    auto loaded = loaded_ok.data;

    title("before relocate");
    RANGE(i, len) { printf("Loaded: %td: %d\n", i, loaded->a[i]); }
    printf("len: %p\n", (void*)loaded->len);
    printf("beg: %p\n", (void*)loaded->beg);
    printf("  a: %p\n", (void*)loaded->a);

    // Reanchor
    loaded->a = (i32*)((char*)loaded->a + (perm2.OrigBeg() - loaded->beg));

    title("after relocate");
    RANGE(i, len) { printf("Loaded: %td: %d\n", i, loaded->a[i]); }
    printf("len: %p\n", (void*)loaded->len);
    printf("beg: %p\n", (void*)loaded->beg);
    printf("  a: %p\n", (void*)loaded->a);

    return 0;
}
