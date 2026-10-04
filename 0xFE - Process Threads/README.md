# Process structure (Linux)

Because "everything is a file" on Linux (chapter `0x01`), a running process
exposes its internals through the **`/proc`** pseudo-filesystem. It is not a real
directory on disk — the kernel generates it on the fly — but you can read it with
ordinary tools like `cat` and `ls`.

Each process has its own directory at `/proc/<pid>/`. A process can also refer to
*itself* as `/proc/self/`.

## Useful entries under /proc/<pid>/

| Path | What it contains |
|------|------------------|
| `cmdline` | The full command line that launched the process (NUL-separated) |
| `environ` | The process's environment variables (NUL-separated) |
| `exe` | A symlink to the executable file on disk |
| `cwd` | A symlink to the current working directory |
| `fd/` | One symlink per open file descriptor (files, sockets, pipes…) |
| `maps` | The memory map: every mapped region, its address range and permissions |
| `mem` | The process's memory itself (read/write via `maps` offsets) |
| `status` | Human-readable state: UID/GID, memory usage, `TracerPid`, threads… |
| `task/` | One subdirectory per **thread** in the process |
| `net/` | Network information as seen by this process |

## Examples

```sh
# What command and arguments is PID 1337 running?
cat /proc/1337/cmdline | tr '\0' ' '; echo

# Read a running process's environment (needs matching privileges)
cat /proc/1337/environ | tr '\0' '\n'

# Where is its executable, and what is its working directory?
ls -l /proc/1337/exe /proc/1337/cwd

# See its memory layout (where the stack, heap and libraries are mapped)
cat /proc/1337/maps

# Is something debugging it? A non-zero TracerPid means yes (see chapter 0x07)
grep TracerPid /proc/1337/status

# List the threads of a process
ls /proc/1337/task/
```

> Reading another user's `/proc/<pid>` is restricted by permissions; you can
> always read your own process via `/proc/self/`. The `fd/` and `maps` entries
> are especially handy in exploitation and forensics for seeing exactly what a
> process has open and where things live in memory.

For more details: https://man7.org/linux/man-pages/man5/proc.5.html
