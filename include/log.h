#pragma once

/* ---------------------------------------------------------------------------
 * Pretty output
 * ------------------------------------------------------------------------- */
#define COLOR_RESET     "\033[0m"
#define COLOR_RED       "\033[0;31m"
#define COLOR_BLUE      "\033[0;34m"
#define COLOR_BLUE_BOLD "\033[1;34m"

#define title(...) (fprintf(stderr, COLOR_BLUE_BOLD), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))
#define debug(...) (fprintf(stderr, COLOR_BLUE), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))
#define error(...)                                                                                                     \
    (fprintf(stderr, COLOR_RED "Error: "), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))

/* ---------------------------------------------------------------------------
 * Error handling
 * ------------------------------------------------------------------------- */
// Technically ...
#define Fatal(err, ...)                                                                                                \
    if ((err) != 0)                                                                                                    \
    {                                                                                                                  \
        error(__VA_ARGS__);                                                                                            \
        debug("[@] %s:%d", __FILE__, __LINE__);                                                                        \
        exit(EXIT_FAILURE);                                                                                            \
    }

// Spicy naming, but ok
#define Return(err, ...)                                                                                               \
    if ((err) != 0)                                                                                                    \
    {                                                                                                                  \
        error(__VA_ARGS__);                                                                                            \
        debug("[@] %s:%d", __FILE__, __LINE__);                                                                        \
        return err;                                                                                                    \
    }
