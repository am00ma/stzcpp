#pragma once

#include "log.h"

#define TEST_SUITE(label)                                                                                              \
    const char* TEST_TITLE = label;                                                                                    \
    int         main()

#define TEST_CASE(label) debug("%s %s %s", TEST_TITLE, " · ", label);

// https://nullprogram.com/blog/2022/06/26/
#define CHECK(cond)                                                                                                    \
    if (!(cond)) { __builtin_trap(); }
