#include "doctest.h"
#include "range.h"

typedef struct List {

    char* buf = 0;
    int   len = 0;

    List() = default;

    List(char* buf_, int len_)
    {
        buf = buf_;
        len = len_;
    }

    template <int N> constexpr List(const char (&s)[N])
    {
        buf = (char*)s;
        len = N - 1;
    }

    // Equality
    bool operator==(List s)
    {
        if (len != s.len)
        {
            // debug("[L] operator==: Not equal");
            return false;
        }

        // Takes care of null vs empty case
        if ((len == 0) && (buf == s.buf))
        {
            // debug("[L] operator==: len = 0 && buf addrs are same");
            return true;
        }

        for (int i = 0; i < len; i++)
        {
            // NOTE: Comparing by copy?
            if (buf[i] != *s[i])
            {
                // debug("[L] operator==: Not equal at %d", i);
                return false;
            }
        }

        // debug("[L] operator==: Equality (len: %d)", len);
        return true;
    }

    // Examples for len: 3
    //   valid:              -3, -2, -1, 0, 1, 2,
    //    null: -inf ... -4,                      3 ... inf
    char* operator[](int i)
    {
        // Bounds check with valid return (just null) - better for testing
        // Returns null if (len == 0)
        if (!((i >= -1 * len) && (i < len))) { return 0; };

        if (i < 0) { i = len + i; } // Negative
        // debug("[L]  len, i >>  %d, %d", len, i);

        return &buf[i]; // Return reference
    }

    // Examples for len: 3
    // Behaviour devided by (left | bottom) / (top & right) / (within table)
    List operator[](int i, int j)
    {
        // debug("[L]  len >>  %d", len);
        // debug("[L] args >> i, j: %d, %d", i, j);

        // Handle left or bottom - return empty
        if ((i >= len) || (j <= -1 * len))
        {
            // debug("[L] left | bottom >> i, j: %d, %d", i, j);
            return List();
        }

        // Handle top & right - return full
        if ((i <= -1 * len) && (j >= len))
        {
            // debug("[L] top & right >> i, j: %d, %d", i, j);
            return *this;
        }

        // Clip i and j to put it with table
        if (j >= len) { j = len; }
        if (i <= -1 * len) { i = -1 * len; }
        // debug("[L]  clip >> i, j: %d, %d", i, j);

        // Negative to positive
        if (i < 0) { i = len + i; }
        if (j < 0) { j = len + j; }
        // debug("[L] - to + >> i, j: %d, %d", i, j);

        // Check overlap given assured both positive (so also for [i, i] for example)
        if (i >= j) { return List(&buf[i], 0); };

        // Guaranteed only if j > i (if i==j, len=cap=0, but address is of ith element)
        return List(&buf[i], j - i);
    }

} List;

int main()
{

    TEST_SUITE("bounds check")
    {
        TEST_CASE("Operator [i]: Single index")
        {

            List a = "hel";
            TEqualInt(a.len, 3);
            TNotNull(a.buf);

            // Positive indices
            TEqualInt(*a[0], 'h');
            TEqualInt(*a[1], 'e');
            TEqualInt(*a[2], 'l');

            // Negative indices
            TEqualInt(*a[-0], 'h');
            TEqualInt(*a[-1], 'l');
            TEqualInt(*a[-2], 'e');
            TEqualInt(*a[-3], 'h');

            // Over bounds
            RANGE(i, 3, 10) { TNull(a[i]); }
            RANGE(i, -10, -3) { TNull(a[i]); }

            // Null String in python:
            //
            // a = ""
            //
            // Traceback (most recent call last):
            //   File "/home/x/hub/repos/study/cpp/stzcpp/docs/fundamentals/python-list.py", line 2, in <module>
            //     a[0]
            //     ~^^^
            // IndexError: string index out of range

            // Empty string
            List b = "";
            TEqualInt(b.len, 0);
            TNotNull(b.buf);

            // Taking index will make return char* of 0
            TNull(b[0]);
            TNull(b[-5]);

            // Null string
            List c = {};
            TEqualInt(c.len, 0);
            TNull(c.buf);
            TNull(c[0]);
            TNull(c[-5]);

            // Same for filled string as well
            TNotNull(a[0]);
            TNull(a[-5]);
        }

        TEST_CASE("Operator [i, j]: Like python")
        {
            List a = "hel";
            TEqualInt(a.len, 3);

            // Need to print table
            List expected[9][9] = {
                {"", "", "h", "he", "", "h", "he", "hel", "hel"}, // -4
                {"", "", "h", "he", "", "h", "he", "hel", "hel"}, // -3
                {"", "", "", "e", "", "", "e", "el", "el"},       // -2
                {"", "", "", "", "", "", "", "l", "l"},           // -1
                {"", "", "h", "he", "", "h", "he", "hel", "hel"}, // 0
                {"", "", "", "e", "", "", "e", "el", "el"},       // -2
                {"", "", "", "", "", "", "", "l", "l"},           // -1
                {"", "", "", "", "", "", "", "", ""},             // 3
                {"", "", "", "", "", "", "", "", ""},             // 4
            };

            // i: row, j: column
            List ret = {};

            // Within confines of table
            RANGE(j, -4, 5)
            RANGE(i, -4, 5)
            {
                ret = a[i, j];
                TEqualStr(ret, expected[i + 4][j + 4]);
            }

            // Outside of table (repeat last/first row/column)

            // Left
            List first_column[9] = {"", "", "", "", "", "", "", "", ""};
            RANGE(j, -5, -4)
            RANGE(i, -4, 5)
            {
                ret = a[i, j];
                TEqualStr(ret, first_column[i + 4]);
            }

            // Right
            List last_column[9] = {"hel", "hel", "el", "l", "hel", "el", "l", "", ""};
            RANGE(j, 5, 10)
            RANGE(i, -4, 5)
            {
                ret = a[i, j];
                TEqualStr(ret, last_column[i + 4]);
            }

            // Top
            List first_row[9] = {"", "", "h", "he", "", "h", "he", "hel", "hel"};
            RANGE(j, -4, 5)
            RANGE(i, -10, -4)
            {
                ret = a[i, j];
                TEqualStr(ret, first_row[j + 4]);
            }

            // Bottom
            List last_row[9] = {"", "", "", "", "", "", "", "", ""};
            RANGE(j, -4, 5)
            RANGE(i, 5, 10)
            {
                ret = a[i, j];
                TEqualStr(ret, last_row[j + 4]);
            }

            // Explicitly listing cases
            TEqualStr((a[0, 0]), List(""));

            // Within table
            TEqualStr((a[0, -2]), List("h"));
            TEqualStr((a[0, -1]), List("he"));

            TEqualStr((a[-2, -2]), List(""));
            TEqualStr((a[-2, -1]), List("e"));

            // Top
            TEqualStr((a[-3, -2]), List("h"));
            TEqualStr((a[-3, -1]), List("he"));

            TEqualStr((a[-4, -2]), List("h"));
            TEqualStr((a[-4, -1]), List("he"));

            // Bottom
            TEqualStr((a[3, -2]), List(""));
            TEqualStr((a[3, -1]), List(""));

            TEqualStr((a[4, -2]), List(""));
            TEqualStr((a[4, -1]), List(""));

            // Left
            TEqualStr((a[-2, -3]), List(""));
            TEqualStr((a[-1, -3]), List(""));

            TEqualStr((a[-2, -4]), List(""));
            TEqualStr((a[-1, -4]), List(""));

            // Right
            TEqualStr((a[-2, 3]), List("el"));
            TEqualStr((a[-1, 3]), List("l"));

            TEqualStr((a[-2, 4]), List("el"));
            TEqualStr((a[-1, 4]), List("l"));
        }

        TEST_CASE("Operator [i]: By reference and by value")
        {
            //
            //
        }
    }

    TEST_RESULTS();
}
