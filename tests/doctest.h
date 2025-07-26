#pragma once

#include "log.h"

// TODO: Can also collect cases in map, so we can properly display at end
#define TEST_SUITE(label)                                                                                              \
    const char* TEST_TITLE          = label;                                                                           \
    int         TEST_CASES          = 0;                                                                               \
    int         TEST_CHECKS         = 0;                                                                               \
    int         TEST_SUCCESS_CASES  = 0;                                                                               \
    int         TEST_SUCCESS_CHECKS = 0;                                                                               \
    title("%s", TEST_TITLE);                                                                                           \
    pretty(COLOR_ITALIC_YELLOW, "  %3s  %-50s %8s  .  %8s ", "", "Case", "Correct", "Total");

// Not sure why I need do while
#define TEST_CASE(label)                                                                                               \
    do {                                                                                                               \
        TEST_CASES++;                                                                                                  \
    } while ((debug("  %3d. %-50s %8d  .  %8d ", TEST_CASES, label, TEST_SUCCESS_CHECKS, TEST_CHECKS),                 \
              (TEST_SUCCESS_CASES += (TEST_SUCCESS_CHECKS == TEST_CHECKS)),                                            \
              ((TEST_CHECKS = 0), (TEST_SUCCESS_CHECKS = 0))) &&                                                       \
             0);

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
#define TEST_RESULTS(...) pretty(COLOR_ITALIC_GREEN, " ✔ %3d / %3d cases", TEST_SUCCESS_CASES, TEST_CASES);
