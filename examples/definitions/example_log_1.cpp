#include "log.h"   // title
#include "str.h"   // Str print, condition checks
#include <cstdio>  // printf inside macros
#include <cstdlib> // Needed for exit (Fatal)

int main()
{

    /* ------------------------------------------------------
     *    Pretty Printing
     *------------------------------------------------------*/

    constexpr char text[]  = "Hello, hi 54.82 ! ✔ ✘ ";
    constexpr char style[] = COLOR_CYAN COLOR_BOLD COLOR_ULINE COLOR_ITALIC;

    title(text);

    debug(text);

    error(text);

    pretty(style, text);

    /* ------------------------------------------------------
     *    Error Handling
     *------------------------------------------------------*/

    Fatal(0, "This will not crash: %d", 0);

    // Fatal(1, "This will crash");

    CheckErr(0, "This will not print anything");

    CheckErr(1, "This will print an error and continue");

    // No need for message, meant to catch in debugger

    Assert(0 == 0); // Will not trigger anything

    // Assert(0 != 0); // Will trigger `__builtin_trap`

    // To be called from within function: Will do nothing if err == 0
    Return(0, "Print an error if err != 0; and return from func");

    // Will print message and return to caller
    // Return(1, "Print an error if err != 0; and return from func");

    /* ------------------------------------------------------
     *    Error Handling
     *------------------------------------------------------*/

    // Check versions print and continue like tests

    // Passing
    int a = 10;
    Equal(a, 10, "%d");
    CheckEqual(a, 10, "%d");
    CheckNotEqual(a, 10, "%d");

    int b = 20;
    NotEqual(a, b, "%d");
    CheckEqual(a, b, "%d");
    CheckNotEqual(a, b, "%d");

    // Checking strings
    Str s1 = "a";
    Str s2 = "b";
    NotEqualStr(s1, s2);
    CheckEqualStr(s1, s2);
    CheckNotEqualStr(s1, s2);

    // Need to type Str("literal")
    CheckEqualStr(s1, Str("a"));
    CheckNotEqualStr(s1, Str("a"));

    return 0;
}
