#pragma once

#include "log.h"  // title, error, pretty
#include <cstdio> // printf, stderr, stdout

// clang-format off
#define TEST_SUITE(label)                                                                                              \
    const char* TEST_TITLE          = label;                                                                           \
    int         TEST_DONE           = 0;                                                                               \
    int         TEST_INDEX          = 0;                                                                               \
    int         TEST_CASES          = 0;                                                                               \
    int         TEST_CHECKS         = 0;                                                                               \
    int         TEST_TOTAL          = 0;                                                                               \
    int         TEST_SUCCESS_CASES  = 0;                                                                               \
    int         TEST_SUCCESS_CHECKS = 0;                                                                               \
    int         TEST_SUCCESS_TOTAL  = 0;                                                                               \
    title("%s", TEST_TITLE);                                                                                           \
    pretty(COLOR_YELLOW, "| %3s  | %-50s | %8s | %8s |", "No.", "Case", "Correct", "Total");                           \
    pretty(COLOR_YELLOW, "| %3s  | %-50s | %8s | %8s |", "---", "----", "-------", "-----");                           \
    for (TEST_DONE = 0; TEST_DONE < 1;                                                                                 \
         (pretty(COLOR_GREEN, "| %3s  | %-50s | %8s | %8s |", "---", "----", "-------", "-----"),                      \
          pretty(COLOR_GREEN, "| %3s  | %37s: %4d / %4d | %8d | %8d |",                                                \
                 (TEST_SUCCESS_CASES == TEST_CASES) ? COLOR_BOLD_GREEN " ✔ " : COLOR_BOLD_RED " ✗ ", "Cases",          \
                  TEST_SUCCESS_CASES, TEST_CASES,                                                                      \
                  TEST_SUCCESS_TOTAL, TEST_TOTAL),                                                                     \
          TEST_DONE++))

#define TEST_CASE(label)                                                                                               \
    for (TEST_INDEX = 0; TEST_INDEX < 1; (pretty(COLOR_RESET, "%s|  %3d | %-50s | %8d | %8d |",                        \
                                                 (TEST_SUCCESS_CHECKS == TEST_CHECKS) ? "" : COLOR_RED,                \
                                                 TEST_CASES + 1, label, TEST_SUCCESS_CHECKS, TEST_CHECKS),             \
                                          TEST_TOTAL += (TEST_CHECKS),                                                 \
                                          TEST_SUCCESS_TOTAL += (TEST_SUCCESS_CHECKS),                                 \
                                          TEST_SUCCESS_CASES += (TEST_SUCCESS_CHECKS == TEST_CHECKS),                  \
                                          TEST_CHECKS = 0, TEST_SUCCESS_CHECKS = 0, TEST_INDEX++, TEST_CASES++))
// clang-format on

// Condition to trigger debugger / crash
// https://nullprogram.com/blog/2022/06/26/
#define TAssert(cond)                                                                                                  \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond)) { __builtin_trap(); }                                                                             \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

// Condition to print, but do not crash
// Diff from doctest: cannot print lhs, rhs separately
#define TCheck(cond)                                                                                                   \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond))                                                                                                   \
        {                                                                                                              \
            pretty(COLOR_YELLOW, "%s:%d", __FILE__, __LINE__);                                                         \
            error("Failed: %s", #cond);                                                                                \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

// Variations on Equal, NotEqual, etc. to count tests
// Diff from doctest: Need to call seperate funcs to print lhs, rhs separately
// So different funcs for positive (==) and negative (!=)

#define TEqual(cond1, cond2, fmt)                                                                                      \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond1 == cond2))                                                                                         \
        {                                                                                                              \
            pretty(COLOR_RED, "\n%s:%d", __FILE__, __LINE__);                                                          \
            pretty(COLOR_YELLOW, "   %s != %s", #cond1, #cond2);                                                       \
            pretty(COLOR_YELLOW, "   " fmt " != " fmt "\n", cond1, cond2);                                             \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

#define TNotEqual(cond1, cond2, fmt)                                                                                   \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond1 != cond2))                                                                                         \
        {                                                                                                              \
            pretty(COLOR_RED, "\n%s:%d", __FILE__, __LINE__);                                                          \
            pretty(COLOR_YELLOW, "   %s == %s", #cond1, #cond2);                                                       \
            pretty(COLOR_YELLOW, "   " fmt " == " fmt "\n", cond1, cond2);                                             \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

#define TEqualStr(str1, str2)                                                                                          \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!((str1) == (str2)))                                                                                       \
        {                                                                                                              \
            pretty(COLOR_RED, "\n%s:%d", __FILE__, __LINE__);                                                          \
            pretty(COLOR_YELLOW, "   %s != %s", #str1, #str2);                                                         \
            pretty(COLOR_YELLOW, "   %.*s != %.*s\n", pstr((str1)), pstr((str2)));                                     \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

#define TNotEqualStr(str1, str2)                                                                                       \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(str1 != str2))                                                                                           \
        {                                                                                                              \
            pretty(COLOR_RED, "\n%s:%d", __FILE__, __LINE__);                                                          \
            pretty(COLOR_YELLOW, "   %s == %s", #str1, #str2);                                                         \
            pretty(COLOR_YELLOW, "   %.*s == %.*s\n", pstr(str1), pstr(str2));                                         \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

// NOTE: Extra shortcut for int, long and addr
#define TEqualInt(cond1, cond2)                                                                                        \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond1 == cond2))                                                                                         \
        {                                                                                                              \
            pretty(COLOR_RED, "\n%s:%d", __FILE__, __LINE__);                                                          \
            pretty(COLOR_YELLOW, "   %s != %s", #cond1, #cond2);                                                       \
            pretty(COLOR_YELLOW, "   %d != %d\n", cond1, cond2);                                                       \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

#define TNotEqualInt(cond1, cond2)                                                                                     \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond1 != cond2))                                                                                         \
        {                                                                                                              \
            pretty(COLOR_RED, "\n%s:%d", __FILE__, __LINE__);                                                          \
            pretty(COLOR_YELLOW, "   %s == %s", #cond1, #cond2);                                                       \
            pretty(COLOR_YELLOW, "   %d == %d\n", cond1, cond2);                                                       \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

#define TEqualLong(cond1, cond2)                                                                                       \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond1 == cond2))                                                                                         \
        {                                                                                                              \
            pretty(COLOR_RED, "\n%s:%d", __FILE__, __LINE__);                                                          \
            pretty(COLOR_YELLOW, "   %s != %s", #cond1, #cond2);                                                       \
            pretty(COLOR_YELLOW, "   %ld != %ld\n", (isize)cond1, (isize)cond2);                                       \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

#define TNotEqualLong(cond1, cond2)                                                                                    \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond1 != cond2))                                                                                         \
        {                                                                                                              \
            pretty(COLOR_RED, "\n%s:%d", __FILE__, __LINE__);                                                          \
            pretty(COLOR_YELLOW, "   %s == %s", #cond1, #cond2);                                                       \
            pretty(COLOR_YELLOW, "   %ld == %ld\n", cond1, cond2);                                                     \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

#define TEqualAddr(cond1, cond2)                                                                                       \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond1 == cond2))                                                                                         \
        {                                                                                                              \
            pretty(COLOR_RED, "\n%s:%d", __FILE__, __LINE__);                                                          \
            pretty(COLOR_YELLOW, "   %s != %s", #cond1, #cond2);                                                       \
            pretty(COLOR_YELLOW, "   %p != %p\n", cond1, cond2);                                                       \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

#define TNotEqualAddr(cond1, cond2)                                                                                    \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond1 != cond2))                                                                                         \
        {                                                                                                              \
            pretty(COLOR_RED, "\n%s:%d", __FILE__, __LINE__);                                                          \
            pretty(COLOR_YELLOW, "   %s == %s", #cond1, #cond2);                                                       \
            pretty(COLOR_YELLOW, "   %p == %p\n", cond1, cond2);                                                       \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

#define TNull(cond1)                                                                                                   \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if ((cond1))                                                                                                   \
        {                                                                                                              \
            pretty(COLOR_RED, "\n%s:%d", __FILE__, __LINE__);                                                          \
            pretty(COLOR_YELLOW, "   %s != (nil)", #cond1);                                                            \
            pretty(COLOR_YELLOW, "   %p != (nil)\n", cond1);                                                           \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

#define TNotNull(cond1)                                                                                                \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond1))                                                                                                  \
        {                                                                                                              \
            pretty(COLOR_RED, "\n%s:%d", __FILE__, __LINE__);                                                          \
            pretty(COLOR_YELLOW, "   %s == (nil)", #cond1);                                                            \
            pretty(COLOR_YELLOW, "   %p == (nil)\n", cond1);                                                           \
        }                                                                                                              \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);
