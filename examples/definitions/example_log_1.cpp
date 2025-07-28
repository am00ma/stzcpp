#include "log.h"   // title
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

    return 0;
}
