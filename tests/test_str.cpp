#include "doctest.h"
#include "str.h"

int main()
{

    TEST_SUITE("Str")
    {

        TEST_CASE("Size: Str")
        {
            CHECK(sizeof(Str) == 16); // 8(buf) + 8(len)
        }

        TEST_CASE("Size: Strs")
        {
            CHECK(sizeof(Strs) == 16); // 8(Str*) + 8(len)
        }

        Arena perm = Arena(1024); // 1KB

        TEST_CASE("Fields: Length of empty string")
        {
            Str x = "";
            CHECK(x.len == 0);
            CHECK(x.buf != 0);

            // When inited with const char*, we get null termination till we interact with arena
            // This will not segfault here
            CHECK(x.buf[x.len] == '\0');
        }

        TEST_CASE("Fields: Length of constant string")
        {
            // However, we register the length without the terminator
            Str x = "123456";
            CHECK(x.len == 6);
        }

        TEST_CASE("Initialization: string literals")
        {
            Str x = "123456";
            CHECK(x.len == 6);
        }

        TEST_CASE("Initialization: const char*")
        {
            const char* s = "123456";

            Str x = Str(s);
            CHECK(x.len == 6);
        }

        TEST_CASE("Initialization: fields")
        {
            char* s = (char*)"123456";
            Str   x = Str(s, strlen(s));
            CHECK(x.len == 6);
        }

        TEST_CASE("Initialization: Arena")
        {
            Arena a = perm;
            Str   x = Str(&a, 6);
            CHECK(x.len == 6); // 6 bytes of 0s, i.e. ""
            CHECK(a.Used() == 6);
        }

        TEST_CASE("Initialization: Formatted string")
        {
            Arena a = perm;

            // needs maxlen
            Str x = Str(&a, 20, "Hello: %s", "hi");

            // Uses only length needed for formatted string
            isize len = 5 + 2 + 2;
            CHECK(x.len == len);
            CHECK(a.Used() == len);
        }

        TEST_CASE("Initialization: Formatted string - non-const")
        {
            Arena a = perm;

            // Format string itself can be created
            Str fmt = Str(&a, 20, "%s: %%10s", "Hello"); // 5 + 2 + 4 = 11
            CHECK(fmt.len == 11);
            char* cfmt = fmt.Cstr(&a); // + 1
            CHECK(strlen(cfmt) == 11); // strlen does not count \0
            CHECK(a.Used() == 12);

            // Further args
            Str x = Str(&a, 20, cfmt, "hi"); // 5 + 2 + 10 = 17
            CHECK(x.len == 17);
            CHECK(a.Used() == 12 + 17);

            cfmt = x.Cstr(&a); // + 1
            CHECK(a.Used() == 12 + 18);
        }

        TEST_CASE("Initialization: Spans")
        {
            Str x = "123456";
            Str y = Str(x.buf, x.buf + 3);
            CHECK(y.len == 3);
            RANGE(i, y.len) { CHECK(y.buf[i] == x.buf[i]); }
        }

        TEST_CASE("Operator: [] - ith char by reference")
        {
            Str   x = "123456";
            char& y = x[0];
            CHECK(y == '1');
            // y = '5'; // Not possible since we have const
        }

        TEST_CASE("Operator: [] - get slice")
        {
            Str x = "123456";
            Str y = x[0, 3];
            CHECK(y.len == 3);
            RANGE(i, y.len) { CHECK(y.buf[i] == x.buf[i]); }
        }

        TEST_CASE("Operator: == - equality")
        {
            Str x = "123456";
            Str y = "123456";
            Str z = "56789";
            CHECK(x == x);
            CHECK(x == y);
            CHECK(x != z);
            CHECK(y != z);
        }

        TEST_CASE("Methods: Cstr - C style null terminated string")
        {
            Arena a = perm;
            Str   x = "123456";

            // Copies entire string to arena
            char* y = x.Cstr(&a);
            CHECK(a.Used() == 7);
            RANGE(i, x.len) { CHECK(y[i] == x.buf[i]); }
            CHECK(y[x.len] == '\0');

            // Reset arena
            a = perm;

            // Copies entire string to arena
            Str z = Str(&a, 5);
            RANGE(i, z.len) { z[i] = i + 65; }
            CHECK(a.Used() == 5);

            // No copy this time
            y = z.Cstr(&a); // + 1, as string is on top of arena
            CHECK(a.Used() == 6);

            // Alloc another string on top
            Str z2 = Str(&a, 5);
            RANGE(i, z2.len) { z[i] = i + 65; }
            CHECK(y[z.len] == '\0');  // Old reference is preserved, with null terminator
            CHECK(a.Used() == 6 + 5); // No null terminator for z2
        }

        TEST_CASE("Methods: Copy - Copy to arena")
        {
            Arena a = perm;
            Str   x = "123456";

            // Does not copy null terminator
            Str y1 = x.Copy(&a);
            CHECK(y1.len == x.len);
            RANGE(i, y1.len) { CHECK(y1.buf[i] == x.buf[i]); }
            CHECK(a.Used() == 6);

            // Reset arena
            a = perm;

            // Can specify to copy with null terminator though
            // NOTE: not reflected in len, but for special occasions where we dont want the tedium of Cstr(...)
            Str y2 = x.Copy(&a, true);
            CHECK(y2.len == x.len);
            RANGE(i, y2.len) { CHECK(y2.buf[i] == x.buf[i]); }
            CHECK(y2.buf[y2.len] == '\0');
            CHECK(a.Used() == 7);
        }

        TEST_CASE("Methods: Copy - Copy to arena - with/without copy")
        {

            Arena a = perm;
            Str   x = "123456";

            // First copy
            Str y1 = x.Copy(&a);
            CHECK(y1.len == x.len);
            RANGE(i, y1.len) { CHECK(y1.buf[i] == x.buf[i]); }
            CHECK(a.Used() == 6);

            // Second copy should not use more memory
            Str y2 = y1.Copy(&a);
            CHECK(a.Used() == 6);

            // Even after renaming variable / copying str
            Str y3 = y2;
            Str y4 = y3.Copy(&a);
            CHECK(a.Used() == 6);

            // However, x is still not on arena, so `x.Copy(...)` will use mem
            Str y5 = x.Copy(&a);
            CHECK(a.Used() == 6 + 6);

            // Cstr also does not copy, just pushes arena up by one and sets zero
            y5.Cstr(&a);
            CHECK(a.Used() == 6 + 6 + 1);

            // Copy with true now will now copy and use one extra space
            // It does not know about null terminated input strings
            // TODO: Can be prevented
            y5.Copy(&a, true);
            CHECK(a.Used() == 6 + 6 + 1 + 7);
        }

        TEST_CASE("Methods: Split - defaults, empty, null")
        {
            Arena a = perm;
            Str   x = "  123456 789 ";

            // ignore_empty = true, substitute_null = false
            const char* res1[] = {"123456", "789"};

            Strs parts1 = x.Split(&a, " ", true, false);
            CHECK(parts1.len == 2);
            RANGE(i, parts1.len) { CHECK(parts1.buf[i] == Str(res1[i])); }

            // ignore_empty = false, substitute_null = false
            const char* res2[] = {"", "", "123456", "789", ""};

            Strs parts2 = x.Split(&a, " ", false, false);
            CHECK(parts2.len == 5);
            RANGE(i, parts2.len) { CHECK(parts2.buf[i] == Str(res2[i])); }

            // ignore_empty = false, substitute_null = true
            const char* res3[] = {"", "", "123456", "789", ""};

            Str  x3     = Str("  123456 789 ").Copy(&a);
            Strs parts3 = x3.Split(&a, " ", false, true);
            CHECK(parts3.len == 5);
            RANGE(i, parts3.len) { CHECK(parts3.buf[i] == Str(res2[i])); }

            CHECK(x3[0] == '\0'); // Confirm replacement with nulls
            CHECK(x3[1] == '\0'); //
        }

        TEST_CASE("Methods: StartsWith")
        {
            Str x = "123456";

            // Positive
            Str y = "123";
            CHECK(x.StartsWith(y));

            // Smaller than y
            x = "12";
            CHECK(!x.StartsWith(y));

            // Null
            x = "";
            CHECK(!x.StartsWith(y));
        }

        TEST_CASE("Methods: EndsWith")
        {
            Str x = "123456";

            // Positive
            Str y = "456";
            CHECK(x.EndsWith(y));

            // Smaller than y
            x = "56";
            CHECK(!x.EndsWith(y));

            // Null
            x = "";
            CHECK(!x.EndsWith(y));
        }

        TEST_CASE("TODO: Methods: Split - multichar separator") { /* TODO */ }

        TEST_CASE("Strs: Initialization: From array")
        {
            Str  list[3] = {"a", "b", "c"};
            Strs parts   = Strs(list, 3); // Needs repetition of '3'
            CHECK(*parts[0] == "a");
            CHECK(*parts[1] == "b");
            CHECK(*parts[2] == "c");

            *parts[1] = "d";         // Mutate
            CHECK(*parts[1] == "d"); // Check mutation
        }

        TEST_CASE("TODO: Strs: Initialization: Default") { /* TODO */ }
        TEST_CASE("TODO: Strs: Initialization: From array") { /* TODO */ }
        TEST_CASE("TODO: Strs: Destructor: Final") { /* TODO */ }

        TEST_CASE("TODO: Strs: operator+: Append") { /* TODO */ }

        TEST_CASE("TODO: Strs: operator[i]: By reference") { /* TODO */ }
        TEST_CASE("TODO: Strs: operator[i, j]: By reference") { /* TODO */ }
        TEST_CASE("TODO: Strs: operator[i, j, a]: By value") { /* TODO */ }
    }

    return 0;
}
