/*
 * Accept string and return parsed, formatted and contatenated string
 */
#include "arena.h"
#include "buf.h"
#include "range.h"
#include <stdio.h>

constexpr isize PERM_CAPACITY = 128;

Str parse_and_format(Str src, Arena temp, Arena* perm)
{
    // Reserve PERM_CAPACITY so str_fmt can use rest
    Buf tmp = Buf(&temp, PERM_CAPACITY);

    // Copy source string ( can only be done before using temp_loop )
    tmp.Join(src);

    // Parse and append to tmp
    RANGE(i, src.len)
    {
        if (src.buf[i] == 'l')
        {
            // Temp arena for this loop
            Arena temp_loop = temp;

            // Format and append ( uses STR_MAXLEN = 1024 )
            Str found = Str(&temp_loop, 32, "Found l: %d\n", i);

            /*
             * temp     :: 0x7fff192c7a70 (beg: 0x56aec34d93b0, end: 0x56aec34d97b0)
             *   left: 1024, used: 128, cap: 1152
             * temp_loop:: 0x7fff192c7a70 (beg: 0x56aec34d93bc, end: 0x56aec34d97b0)
             *   left: 1012, used: 140, cap: 1152
             *
             * 140 - 128 = 12 extra for `Found ... <10`
             * 141 - 128 = 13 extra for `Found ... >=10`
             */

            // Store
            tmp.Join(found); // copies memory

            printf("\n--- found l >> i = %ld ---\n", i);

            temp.Print("temp     :");
            temp_loop.Print("temp_loop:");
        }
    }

    printf("\n-------------\n");

    // Finalize temp arena ( Not neccessary, but frees STR_MAXLEN stuff )
    Str out = tmp.Final(&temp);
    temp.Print("temp     :");

    // Copy to permanent arena
    Str dst = out.Copy(perm);

    return dst;
}

int main()
{
    // ------------------------------------
    Arena perm = Arena(1024); // 1KB
    perm.Print("Initial(perm)");

    Arena temp = Arena(1024); // 1KB
    temp.Print("Initial(temp)");

    Str src = "hello hi, alles good?\n";
    Str dst = parse_and_format(src, temp, &perm);
    printf("%.*s", pstr(dst));

    perm.Print("After parse_and_format(perm):");
    temp.Print("After parse_and_format(temp):");

    return 0;
}
