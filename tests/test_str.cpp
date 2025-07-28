#include "doctest.h"
#include "str.h"

int main()
{

    TEST_SUITE("Str")
    {

        TEST_CASE("Size: Str")
        {
            TCheck(sizeof(Str) == 16); // 8(buf) + 8(len)
        }

        TEST_CASE("Size: Strs")
        {
            TCheck(sizeof(Strs) == 24); // 8(Str*) + 8(len) + 8(cap)
        }

        Arena perm = Arena(1024); // 1KB

        TEST_CASE("Fields: Length of empty string")
        {
            Str x = "";
            TCheck(x.len == 0);
            TCheck(x.buf != 0);

            // When inited with const char*, we get null termination till we interact with arena
            // This will not segfault here
            TCheck(x.buf[x.len] == '\0');
        }

        TEST_CASE("Fields: Length of constant string")
        {
            // However, we register the length without the terminator
            Str x = "123456";
            TCheck(x.len == 6);
        }

        TEST_CASE("Initialization: string literals")
        {
            Str x = "123456";
            TCheck(x.len == 6);
        }

        TEST_CASE("Initialization: const char*")
        {
            const char* s = "123456";

            Str x = Str(s);
            TCheck(x.len == 6);
        }

        TEST_CASE("Initialization: fields")
        {
            char* s = (char*)"123456";
            Str   x = Str(s, strlen(s));
            TCheck(x.len == 6);
        }

        TEST_CASE("Initialization: Arena")
        {
            Arena a = perm;
            Str   x = Str(&a, 6);
            TCheck(x.len == 6); // 6 bytes of 0s, i.e. ""
            TCheck(a.Used() == 6);
        }

        TEST_CASE("Initialization: Formatted string")
        {
            Arena a = perm;

            // needs maxlen
            Str x = Str(&a, 20, "Hello: %s", "hi");

            // Uses only length needed for formatted string
            isize len = 5 + 2 + 2;
            TCheck(x.len == len);
            TCheck(a.Used() == len);
        }

        TEST_CASE("Initialization: Formatted string - non-const")
        {
            Arena a = perm;

            // Format string itself can be created
            Str fmt = Str(&a, 20, "%s: %%10s", "Hello"); // 5 + 2 + 4 = 11
            TCheck(fmt.len == 11);
            char* cfmt = fmt.Cstr(&a);  // + 1
            TCheck(strlen(cfmt) == 11); // strlen does not count \0
            TCheck(a.Used() == 12);

            // Further args
            Str x = Str(&a, 20, cfmt, "hi"); // 5 + 2 + 10 = 17
            TCheck(x.len == 17);
            TCheck(a.Used() == 12 + 17);

            cfmt = x.Cstr(&a); // + 1
            TCheck(a.Used() == 12 + 18);
        }

        TEST_CASE("Initialization: Spans")
        {
            Str x = "123456";
            Str y = Str(x.buf, x.buf + 3);
            TCheck(y.len == 3);
            RANGE(i, y.len) { TCheck(y.buf[i] == x.buf[i]); }
        }

        TEST_CASE("Operator: [] - ith char by reference")
        {
            Str   x = "123456";
            char& y = x[0];
            TCheck(y == '1');
            // y = '5'; // Not possible since we have const
        }

        TEST_CASE("Operator: [] - get slice")
        {
            Str x = "123456";
            Str y = x[0, 3];
            TCheck(y.len == 3);
            RANGE(i, y.len) { TCheck(y.buf[i] == x.buf[i]); }
        }

        TEST_CASE("Operator: == - equality")
        {
            Str x = "123456";
            Str y = "123456";
            Str z = "56789";
            TCheck(x == x);
            TCheck(x == y);
            TCheck(x != z);
            TCheck(y != z);
        }

        TEST_CASE("Methods: Cstr - C style null terminated string")
        {
            Arena a = perm;
            Str   x = "123456";

            // Copies entire string to arena
            char* y = x.Cstr(&a);
            TCheck(a.Used() == 7);
            RANGE(i, x.len) { TCheck(y[i] == x.buf[i]); }
            TCheck(y[x.len] == '\0');

            // Reset arena
            a = perm;

            // Copies entire string to arena
            Str z = Str(&a, 5);
            RANGE(i, z.len) { z[i] = i + 65; }
            TCheck(a.Used() == 5);

            // No copy this time
            y = z.Cstr(&a); // + 1, as string is on top of arena
            TCheck(a.Used() == 6);

            // Alloc another string on top
            Str z2 = Str(&a, 5);
            RANGE(i, z2.len) { z[i] = i + 65; }
            TCheck(y[z.len] == '\0');  // Old reference is preserved, with null terminator
            TCheck(a.Used() == 6 + 5); // No null terminator for z2
        }

        TEST_CASE("Methods: Copy - Copy to arena")
        {
            Arena a = perm;
            Str   x = "123456";

            // Does not copy null terminator
            Str y1 = x.Copy(&a);
            TCheck(y1.len == x.len);
            RANGE(i, y1.len) { TCheck(y1.buf[i] == x.buf[i]); }
            TCheck(a.Used() == 6);

            // Reset arena
            a = perm;

            // Can specify to copy with null terminator though
            // NOTE: not reflected in len, but for special occasions where we dont want the tedium of Cstr(...)
            Str y2 = x.Copy(&a, true);
            TCheck(y2.len == x.len);
            RANGE(i, y2.len) { TCheck(y2.buf[i] == x.buf[i]); }
            TCheck(y2.buf[y2.len] == '\0');
            TCheck(a.Used() == 7);
        }

        TEST_CASE("Methods: Copy - Copy to arena - with/without copy")
        {

            Arena a = perm;
            Str   x = "123456";

            // First copy
            Str y1 = x.Copy(&a);
            TCheck(y1.len == x.len);
            RANGE(i, y1.len) { TCheck(y1.buf[i] == x.buf[i]); }
            TCheck(a.Used() == 6);

            // Second copy should not use more memory
            Str y2 = y1.Copy(&a);
            TCheck(a.Used() == 6);

            // Even after renaming variable / copying str
            Str y3 = y2;
            Str y4 = y3.Copy(&a);
            TCheck(a.Used() == 6);

            // However, x is still not on arena, so `x.Copy(...)` will use mem
            Str y5 = x.Copy(&a);
            TCheck(a.Used() == 6 + 6);

            // Cstr also does not copy, just pushes arena up by one and sets zero
            y5.Cstr(&a);
            TCheck(a.Used() == 6 + 6 + 1);

            // Copy with true now will now copy and use one extra space
            // It does not know about null terminated input strings
            // TODO: Can be prevented
            y5.Copy(&a, true);
            TCheck(a.Used() == 6 + 6 + 1 + 7);
        }

        TEST_CASE("Methods: StartsWith")
        {
            Str x = "123456";

            // Positive
            Str y = "123";
            TCheck(x.StartsWith(y));

            // Smaller than y
            x = "12";
            TCheck(!x.StartsWith(y));

            // Null
            x = "";
            TCheck(!x.StartsWith(y));
        }

        TEST_CASE("Methods: EndsWith")
        {
            Str x = "123456";

            // Positive
            Str y = "456";
            TCheck(x.EndsWith(y));

            // Smaller than y
            x = "56";
            TCheck(!x.EndsWith(y));

            // Null
            x = "";
            TCheck(!x.EndsWith(y));
        }

        TEST_CASE("Methods: Split - defaults, empty, null")
        {
            Arena a = perm;
            Str   x = "  123456 789 ";

            isize max_parts = 16; // Default is 1024

            // ignore_empty = true, substitute_null = false
            const char* res1[] = {"123456", "789"};

            Strs parts1 = x.Split(&a, " ", true, false, max_parts);
            TCheck(parts1.len == 2);
            RANGE(i, parts1.len) { TCheck(parts1.buf[i] == Str(res1[i])); }

            // ignore_empty = false, substitute_null = false
            const char* res2[] = {"", "", "123456", "789", ""};

            Strs parts2 = x.Split(&a, " ", false, false, max_parts);
            TCheck(parts2.len == 5);
            RANGE(i, parts2.len) { TCheck(parts2.buf[i] == Str(res2[i])); }

            // ignore_empty = false, substitute_null = true
            const char* res3[] = {"", "", "123456", "789", ""};

            Str  x3     = Str("  123456 789 ").Copy(&a);
            Strs parts3 = x3.Split(&a, " ", false, true, max_parts);
            TCheck(parts3.len == 5);
            RANGE(i, parts3.len) { TCheck(parts3.buf[i] == Str(res2[i])); }

            TCheck(x3[0] == '\0'); // Confirm replacement with nulls
            TCheck(x3[1] == '\0'); //
        }

        TEST_CASE("TODO: Methods: Split - multichar separator") { /* TODO */ }
    }

    return 0;
}
