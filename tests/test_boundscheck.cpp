#include "doctest.h"
#include "log.h"
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
            // debug("[L] Found False");
            return false;
        }
        if ((len == 0) && (buf == s.buf)) { return true; } // Takes care of null vs empty case
        for (int i = 0; i < len; i++)
        {
            if (buf[i] != *s[i]) return false; // Comparing by copy?
        }
        return true;
    }

    char* operator[](int i)
    {
        // Examples:
        //   len: 3
        //    in: -3, 0, 2
        //   out: -inf ... -4
        //   out: 3 ... inf

        if (!((i >= -1 * len) && (i < len))) { return 0; }; // Bounds check with valid return (just null)

        // AssertMsg((i >= -1 * len) && (i < len), // Bounds check
        //           "\ni = %d\n"                  //
        //           "  |           len = %d\n"    //
        //           "  | i >= -1 * len = %d\n"    // Helpful output
        //           "  |       i < len = %d\n",   //
        //           i, len, i >= -1 * len, i < len);

        if (i < 0) { i = len + i; } // Negative

        return &buf[i]; // Return reference
    }

    List operator[](int i, int j)
    {
        // debug("[L]  len >>  %d", len);
        // debug("[L] args >> i, j: %d, %d", i, j);

        // Handle left or bottom - return empty
        if ((i >= len) || (j <= -1 * len))
        {
            // debug("[L] left or bottom >> i, j: %d, %d", i, j);
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
    }

    return 0;
}
