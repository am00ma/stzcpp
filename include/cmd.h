#pragma once

#include "str.h"

typedef enum {
    CMD_SUCCESS = 0,
    CMD_FAIL_OOM,
    CMD_FAIL_PIPE,
    CMD_FAIL_FORK,
    CMD_FAIL_UNKNOWN,
} CmdError;

typedef struct Cmd {
    Str stdout;
    Str stderr;
    int status;
} Cmd;

// Run with default output of 16 MB, error of 2 MB
Result<Cmd, CmdError> Cmd_Exec(Arena* perm, Str command,
                               isize out_maxlen = 16 * 1024 * 1024, // stdout: 16MB
                               isize err_maxlen = 2 * 1024 * 1024,  // stderr:  2MB
                               Str   shell      = "/bin/sh");
