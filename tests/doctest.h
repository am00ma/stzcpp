#pragma once

#include "log.h"

// TODO: Can remormat to for loop as well
#define TEST_SUITE(label)                                                                                              \
    const char* TEST_TITLE          = label;                                                                           \
    int         TEST_INDEX          = 0;                                                                               \
    int         TEST_CASES          = 0;                                                                               \
    int         TEST_CHECKS         = 0;                                                                               \
    int         TEST_SUCCESS_CASES  = 0;                                                                               \
    int         TEST_SUCCESS_CHECKS = 0;                                                                               \
    title("%s", TEST_TITLE);                                                                                           \
    pretty(COLOR_ITALIC_YELLOW, "  %3s  %-50s %8s  .  %8s ", "", "Case", "Correct", "Total");

// Putting statements at end of for loop makes them execute after scope

// clang-format off
#define TEST_CASE(label)                                                                                               \
    for (TEST_INDEX = 0; TEST_INDEX < 1;                                                                               \
            (                                                                                                          \
            pretty(COLOR_RESET, "  %3d. %-50s %8d  .  %8d ", TEST_CASES+1, label, TEST_SUCCESS_CHECKS, TEST_CHECKS),   \
            TEST_SUCCESS_CASES += (TEST_SUCCESS_CHECKS == TEST_CHECKS),                                                \
            TEST_CHECKS = 0, TEST_SUCCESS_CHECKS = 0,                                                                  \
            TEST_INDEX++,                                                                                              \
            TEST_CASES++                                                                                               \
            )                                                                                                          \
        )
// clang-format on

// https://nullprogram.com/blog/2022/06/26/
#define ASSERT(cond)                                                                                                   \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond)) { __builtin_trap(); }                                                                             \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

// Register mismatch, but do not crash
#define CHECK(cond)                                                                                                    \
    do {                                                                                                               \
        TEST_CHECKS++;                                                                                                 \
        if (!(cond)) { error("Failed: %s", #cond); }                                                                   \
        else { TEST_SUCCESS_CHECKS++; }                                                                                \
    } while (0);

// Print test results
#define TEST_RESULTS(...) pretty(COLOR_ITALIC_GREEN, "    ✔ %3d / %3d cases", TEST_SUCCESS_CASES, TEST_CASES);
