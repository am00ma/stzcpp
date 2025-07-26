#pragma once

#include "log.h"

#define TEST_SUITE(label)                                                                                              \
    const char* TEST_TITLE          = label;                                                                           \
    int         TEST_CASES          = 0;                                                                               \
    int         TEST_CHECKS         = 0;                                                                               \
    int         TEST_SUCCESS_CASES  = 0;                                                                               \
    int         TEST_SUCCESS_CHECKS = 0;

#define TEST_CASE(label)                                                                                               \
    TEST_CASES++;                                                                                                      \
    debug("%s %s %s", TEST_TITLE, " · ", label);

// https://nullprogram.com/blog/2022/06/26/
#define ASSERT(cond)                                                                                                   \
    TEST_CHECKS++;                                                                                                     \
    if (!(cond)) { __builtin_trap(); }

// Show, but move on
#define CHECK(cond)                                                                                                    \
    TEST_CHECKS++;                                                                                                     \
    if (!(cond)) { error("Failed: %s", #cond); }

#define TEST_RESULTS(...)                                                                                              \
    debug("%s", TEST_TITLE);                                                                                           \
    debug("  Checks: %3d / %3d", TEST_SUCCESS_CHECKS, TEST_CHECKS);                                                    \
    debug("   Cases: %3d / %3d", TEST_SUCCESS_CASES, TEST_CASES);
