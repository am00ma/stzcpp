#include "util/cmd.h"

#include <sys/wait.h> // waitpid, pid
#include <unistd.h>   // pipe, dup2, fork, close

Result<Cmd, CmdError> Cmd_Exec(Arena* perm, Str command, isize outlen, isize errlen, Str shell)
{
    // Get null terminated string of command
    const char* command_cstr = command.Cstr(perm);
    const char* shell_cstr   = shell.Cstr(perm);

    // Default result
    Result<Cmd, CmdError> res = {{}, CMD_FAIL_UNKNOWN};

    int   stdout_pipe[2], stderr_pipe[2];
    pid_t pid;

    // Create pipes for stdout and stderr
    if (pipe(stdout_pipe) == -1 || pipe(stderr_pipe) == -1)
    {
        perror("pipe");
        return {{}, CMD_FAIL_PIPE};
    }

    // Fork the process
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return {{}, CMD_FAIL_FORK};
    }

    if (pid == 0)
    { // Child process
        // Redirect stdout to the write end of the stdout pipe
        dup2(stdout_pipe[1], STDOUT_FILENO);
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);

        // Redirect stderr to the write end of the stderr pipe
        dup2(stderr_pipe[1], STDERR_FILENO);
        close(stderr_pipe[0]);
        close(stderr_pipe[1]);

        // Execute the command
        int err = execl(shell_cstr, shell_cstr, "-c", command_cstr, (char*)NULL);
        if (err)
        {
            perror("execl"); // If execl fails
            exit(EXIT_FAILURE);
        }
    }
    else
    { // Parent process
        // Close the write ends of the pipes
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        // Keep track of capacity for out of memory error
        // Fortunately, capacity is signed (ptrdiff_t)
        isize cap, num_bytes;

        // Read stdout from the pipe NOTE: Pointer arithmatic
        Buf out   = {perm, outlen};
        cap       = out.cap;
        num_bytes = 0;
        do {
            if (cap <= 0) return {{}, CMD_FAIL_UNKNOWN};
            num_bytes  = read(stdout_pipe[0], out.buf + out.len, cap);
            out.len   += num_bytes;
            cap       -= num_bytes;
        } while (num_bytes != 0);
        out.Shrink(perm);
        res.data.out = {out.buf, out.len};
        close(stdout_pipe[0]);

        // Read stderr from the pipe NOTE: Pointer arithmatic
        Buf err   = {perm, errlen};
        num_bytes = 0;
        cap       = err.cap;
        do {
            if (cap <= 0) return {{}, CMD_FAIL_UNKNOWN};
            num_bytes  = read(stderr_pipe[0], err.buf + err.len, cap);
            err.len   += num_bytes;
            cap       -= num_bytes;
        } while (num_bytes != 0);
        err.Shrink(perm);
        res.data.err = {err.buf, err.len};
        close(stderr_pipe[0]);

        // Wait for the child process to finish and get the exit status
        waitpid(pid, (int*)&res.data.status, 0);
        if (WIFEXITED(res.data.status)) { res.data.status = WEXITSTATUS(res.data.status); }
    }

    res.err = CMD_SUCCESS;
    return res;
}
