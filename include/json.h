#pragma once

#include "str.h"

#include <rapidjson/document.h>

// NOTE: Placeholder
typedef struct KeyVal {
    Str key;
    Str val;

    KeyVal() = default; // Default constructor

    // From json string ((key.len == 0) => entry is invalid)
    KeyVal(Str json);

    // To json string (cannot fail)
    Str Json();

} Task;
