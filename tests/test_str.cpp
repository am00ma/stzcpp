#include "str.h"

#include "doctest.h"

int main()
{

    TEST_SUITE("Str")
    {

        TEST_CASE("Stuct size: Str")
        {
            CHECK(sizeof(Str) == 16); // 8(buf) + 8(len)
        }

        TEST_CASE("Stuct size: Strs")
        {
            CHECK(sizeof(Strs) == 16); // 8(Str*) + 8(len)
        }

        Arena perm = Arena(1024); // 1KB

        TEST_CASE("Length of empty string")
        {
            Str x = "";
            CHECK(x.len == 0);
        }

        TEST_CASE("Length of constant string")
        {
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
        }

        TEST_CASE("Initialization: Formatted string")
        {
            Arena a = perm;
            Str   x = Str(&a, 20, "Hello: %s", "hi"); // needs maxlen
            CHECK(x.len == 5 + 2 + 2);
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
            // y = '5'; // Not possible
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
            char* y = x.Cstr(&a);
            RANGE(i, x.len) { CHECK(y[i] == x.buf[i]); }
            CHECK(y[x.len] == '\0');
        }

        TEST_CASE("Methods: Copy - Copy to arena")
        {
            Arena a = perm;
            Str   x = "123456";

            Str y1 = x.Copy(&a);
            CHECK(y1.len == x.len);
            RANGE(i, y1.len) { CHECK(y1.buf[i] == x.buf[i]); }

            Str y2 = x.Copy(&a, true);
            CHECK(y2.len == x.len);
            RANGE(i, y2.len) { CHECK(y2.buf[i] == x.buf[i]); }
            CHECK(y2.buf[y2.len] == '\0');
        }

        TEST_CASE("Methods: Copy - Copy to arena - without copy") { /* TODO */ }
        TEST_CASE("Methods: Copy - Copy to arena - with copy") { /* TODO */ }

        TEST_CASE("Methods: Split - Default, ignore_empty, substitute_null")
        {
            Arena a = perm;
            Str   x = "  123456 789 ";

            // ignore_empty = true, substitute_null = false
            Strs parts1 = x.Split(&a, " ", true, false);
            CHECK(parts1.len == 2);
            CHECK(parts1.data[0] == Str("123456"));
            CHECK(parts1.data[1] == Str("789"));

            // ignore_empty = false, substitute_null = false
            const char* res2[] = {"", "", "123456", "789", ""};

            Strs parts2 = x.Split(&a, " ", false, false);
            CHECK(parts2.len == 5);
            RANGE(i, parts2.len) { CHECK(parts2.data[i] == Str(res2[i])); }

            // ignore_empty = false, substitute_null = true
            const char* res3[] = {"", "", "123456", "789", ""};
            Str         x3     = Str("  123456 789 ").Copy(&a);
            Strs        parts3 = x3.Split(&a, " ", false, true);
            CHECK(parts3.len == 5);
            RANGE(i, parts3.len) { CHECK(parts3.data[i] == Str(res2[i])); }
            CHECK(x3[0] == '\0');
            CHECK(x3[1] == '\0');
        }

        TEST_CASE("Methods: Split - multichar separator") { /* TODO */ }

        TEST_CASE("Strs: Initialization")
        {
            Str  list[3] = {"a", "b", "c"};
            Strs parts   = Strs(list, 3);
            CHECK(*parts[0] == Str("a"));
            CHECK(*parts[1] == Str("b"));
            CHECK(*parts[2] == Str("c"));
            *parts[1] = "d";
            CHECK(*parts[1] == Str("d"));
        }
    }

    TEST_RESULTS();

    return 0;
}
