#pragma once

/* ---------------------------------------------------------------------------
 * Pretty output
 * ------------------------------------------------------------------------- */
#define COLOR_RESET      "\033[0m"
#define COLOR_RED        "\033[0;31m"
#define COLOR_GREEN      "\033[0;32m"
#define COLOR_BLUE       "\033[0;34m"
#define COLOR_BLUE_BOLD  "\033[1;34m"
#define COLOR_BOLD_RED   "\033[1;31m"
#define COLOR_BOLD_BLUE  "\033[1;34m"
#define COLOR_BOLD_GREEN "\033[1;32m"

#define title(...) (fprintf(stderr, COLOR_BLUE_BOLD), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))
#define debug(...) (fprintf(stderr, COLOR_BLUE), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))
#define error(...) (fprintf(stderr, COLOR_RED "[E] "), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))

/* ---------------------------------------------------------------------------
 * Error handling
 * ------------------------------------------------------------------------- */
// Crash entirely
#define Fatal(err, ...)                                                                                                \
    if ((err) != 0)                                                                                                    \
    {                                                                                                                  \
        error(__VA_ARGS__);                                                                                            \
        debug("[@] %s:%d", __FILE__, __LINE__);                                                                        \
        exit(EXIT_FAILURE);                                                                                            \
    }

// Print error but continue
#define CheckErr(err, ...)                                                                                             \
    if ((err) != 0)                                                                                                    \
    {                                                                                                                  \
        error(__VA_ARGS__);                                                                                            \
        debug("[@] %s:%d", __FILE__, __LINE__);                                                                        \
    }

// Print error and return to parent
#define Return(err, ...)                                                                                               \
    if ((err) != 0)                                                                                                    \
    {                                                                                                                  \
        error(__VA_ARGS__);                                                                                            \
        debug("[@] %s:%d", __FILE__, __LINE__);                                                                        \
        return err;                                                                                                    \
    }

// Debugger friendly assert
// https://nullprogram.com/blog/2022/06/26/
#define Assert(cond)                                                                                                   \
    if (!(cond))                                                                                                       \
    {                                                                                                                  \
        error("[@] %s:%d", __FILE__, __LINE__);                                                                        \
        __builtin_trap();                                                                                              \
    }
