#include "doctest.h"

typedef struct List {

    char* buf = 0;
    int   len = 0;

    List() = default;

    template <int N> constexpr List(const char (&s)[N])
    {
        buf = (char*)s;
        len = N;
    }

    char* operator[](int i)
    {
        if (!((i >= -1 * len) && (i < len))) { return 0; }; // Bounds check
        if (i < 0) { i = len + i; }                         // Negative
        return &buf[i];                                     // Return reference
    }

} List;

int main()
{

    TEST_SUITE("bounds check")
    {
        TEST_CASE("Operator [i]: Single index")
        {

            List a = "hello";
            TEqualInt(a.len, 6);
            TNotNull(a.buf);
            TNotNull(&a.buf[0]);

            // Positive indices
            TEqualInt(*a[0], 'h');
            TEqualInt(*a[1], 'e');
            TEqualInt(*a[2], 'l');
            TEqualInt(*a[3], 'l');
            TEqualInt(*a[4], 'o');
            TEqualInt(*a[5], '\0');

            // Negative indices
            TEqualInt(*a[-0], 'h');
            TEqualInt(*a[-1], '\0');
            TEqualInt(*a[-2], 'o');
            TEqualInt(*a[-3], 'l');
            TEqualInt(*a[-4], 'l');
            TEqualInt(*a[-5], 'e');
            TEqualInt(*a[-6], 'h');

            // Over bounds
            TNull(a[6]);
            TNull(a[10]);

            // Under bounds
            TNull(a[-7]);
            TNull(a[-10]);
        }
    }

    return 0;
}
