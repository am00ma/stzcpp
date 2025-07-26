#pragma once

/* ---------------------------------------------------------------------------
 * Pretty output
 * ------------------------------------------------------------------------- */
#define COLOR_RESET "\033[0m"

#define COLOR_BLACK   "\033[0;30m"
#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_CYAN    "\033[0;36m"
#define COLOR_WHITE   "\033[0;37m"

#define COLOR_BOLD_BLACK   "\033[1;30m"
#define COLOR_BOLD_RED     "\033[1;31m"
#define COLOR_BOLD_GREEN   "\033[1;32m"
#define COLOR_BOLD_YELLOW  "\033[1;33m"
#define COLOR_BOLD_BLUE    "\033[1;34m"
#define COLOR_BOLD_MAGENTA "\033[1;35m"
#define COLOR_BOLD_CYAN    "\033[1;36m"
#define COLOR_BOLD_WHITE   "\033[1;37m"

#define COLOR_ITALIC_BLACK   "\033[3;30m"
#define COLOR_ITALIC_RED     "\033[3;31m"
#define COLOR_ITALIC_GREEN   "\033[3;32m"
#define COLOR_ITALIC_YELLOW  "\033[3;33m"
#define COLOR_ITALIC_BLUE    "\033[3;34m"
#define COLOR_ITALIC_MAGENTA "\033[3;35m"
#define COLOR_ITALIC_CYAN    "\033[3;36m"
#define COLOR_ITALIC_WHITE   "\033[3;37m"

#define COLOR_ULINE_BLACK   "\033[4;30m"
#define COLOR_ULINE_RED     "\033[4;31m"
#define COLOR_ULINE_GREEN   "\033[4;32m"
#define COLOR_ULINE_YELLOW  "\033[4;33m"
#define COLOR_ULINE_BLUE    "\033[4;34m"
#define COLOR_ULINE_MAGENTA "\033[4;35m"
#define COLOR_ULINE_CYAN    "\033[4;36m"
#define COLOR_ULINE_WHITE   "\033[4;37m"

#define COLOR_BG_BLACK   "\033[40m"
#define COLOR_BG_RED     "\033[41m"
#define COLOR_BG_GREEN   "\033[42m"
#define COLOR_BG_YELLOW  "\033[43m"
#define COLOR_BG_BLUE    "\033[44m"
#define COLOR_BG_MAGENTA "\033[45m"
#define COLOR_BG_CYAN    "\033[46m"
#define COLOR_BG_WHITE   "\033[47m"

#define title(...)                                                                                                     \
    (fprintf(stderr, COLOR_BOLD_YELLOW COLOR_ITALIC_YELLOW COLOR_ULINE_YELLOW), fprintf(stderr, __VA_ARGS__),          \
     fprintf(stderr, COLOR_RESET "\n"))
#define debug(...) (fprintf(stderr, COLOR_BLUE), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))
#define error(...) (fprintf(stderr, COLOR_RED "[E] "), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))

#define pretty(color, ...) (fprintf(stderr, color), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))

/* ---------------------------------------------------------------------------
 * Error handling
 * ------------------------------------------------------------------------- */
// Crash entirely
#define Fatal(err, ...)                                                                                                \
    if ((err) != 0)                                                                                                    \
    {                                                                                                                  \
        error(__VA_ARGS__);                                                                                            \
        error("%s:%d", __FILE__, __LINE__);                                                                            \
        exit(EXIT_FAILURE);                                                                                            \
    }

// Print error but continue
#define CheckErr(err, ...)                                                                                             \
    if ((err) != 0)                                                                                                    \
    {                                                                                                                  \
        error(__VA_ARGS__);                                                                                            \
        error("%s:%d", __FILE__, __LINE__);                                                                            \
    }

// Print error and return to parent
#define Return(err, ...)                                                                                               \
    if ((err) != 0)                                                                                                    \
    {                                                                                                                  \
        error(__VA_ARGS__);                                                                                            \
        error("%s:%d", __FILE__, __LINE__);                                                                            \
        return err;                                                                                                    \
    }

// Debugger friendly assert
// https://nullprogram.com/blog/2022/06/26/
#define Assert(cond)                                                                                                   \
    if (!(cond))                                                                                                       \
    {                                                                                                                  \
        error("%s:%d", __FILE__, __LINE__);                                                                            \
        __builtin_trap();                                                                                              \
    }
