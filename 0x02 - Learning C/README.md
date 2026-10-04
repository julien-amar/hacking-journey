# Introduction to C

C matters for security work because it compiles almost directly to machine code,
it lets you manipulate memory by hand, and most of the operating system (and the
classic vulnerable programs) are written in it. Understanding C is the bridge
between source code and the assembly you will read in a debugger.

## Hello world in C

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc == 2)
    {
        printf("Knock, knock, %s\n", argv[1]);
    }
    else
    {
        fprintf(stderr, "Usage: %s <name>\n", argv[0]);
        return 1;
    }
    return 0;
}
```

* `main` is the entry point of the program.
* `argc` is the number of arguments passed to the executable (the program name
  itself counts, so it is always at least `1`).
* `argv` is the array of argument strings; `argv[0]` is the program name,
  `argv[1]` the first real argument, and so on.

All standard C functions are documented in `man`. For example, `printf` lives in
manual section 3 (library functions): `man 3 printf`.

## Compilation

To compile a C program you need a compiler (`cc`, `gcc`, or `clang`). `-Wall`
turns on common warnings — always keep it on:

```sh
$ gcc hello.c -o hello -Wall

$ ./hello
Usage: ./hello <name>

$ ./hello Neo
Knock, knock, Neo

# The program expects exactly 1 extra argument
$ ./hello Neo the hero
Usage: ./hello <name>

# Pass a single string (with spaces) by quoting it
$ ./hello "Neo the hero"
Knock, knock, Neo the hero

# The shell expands $USER before the program sees it
$ ./hello $USER
Knock, knock, ange

# Escape or single-quote to pass the literal text "$USER"
$ ./hello \$USER
Knock, knock, $USER

$ ./hello '$USER'
Knock, knock, $USER
```

### Useful compilation flags

For reverse-engineering and exploitation practice you often want to *disable*
the compiler's protections so the behaviour is easy to observe:

```sh
# -g            include debug symbols (names, line numbers) for GDB
# -O0           no optimisation, so the assembly matches the source
# -fno-stack-protector   remove the stack canary (see chapter 0x0A)
# -z execstack  make the stack executable
# -no-pie       produce a non-position-independent executable (fixed addresses)
gcc -g -O0 -fno-stack-protector -z execstack -no-pie vuln.c -o vuln
```

> These flags re-create the "easy mode" conditions many exploitation tutorials
> assume. Real modern binaries have all of these protections enabled.

## Return value

A process returns an integer exit code (0 means success by convention). The
shell exposes the last program's exit code through the `$?` variable:

```sh
$ ./hello Neo
Knock, knock, Neo
$ echo $?
0
$ ./hello
Usage: ./hello <name>
$ echo $?
1
```

## Pointers, memory and buffers

Almost every classic memory-corruption bug comes down to C letting you read or
write memory that you shouldn't. A few essentials:

* A **pointer** is a variable that holds a memory address. `*p` reads/writes the
  value at that address; `&x` gives the address of `x`.
* An **array / buffer** is a contiguous block of memory. `char buf[64]` reserves
  64 bytes on the stack — but C does *not* check whether you stay inside those
  64 bytes.
* Writing past the end of a buffer (a **buffer overflow**) overwrites whatever
  happens to be next in memory: other variables, saved registers, or the return
  address. That is the foundation of chapters `0x0A` and `0x0D`.

Some standard functions are dangerous precisely because they don't take a size
limit and will happily overflow a buffer:

| Dangerous | Safer alternative | Why |
|-----------|-------------------|-----|
| `gets(buf)` | `fgets(buf, size, stdin)` | `gets` has no size limit at all |
| `strcpy(dst, src)` | `strncpy` / `snprintf` | copies until a NUL byte, ignoring `dst` size |
| `strcat(dst, src)` | `strncat` / `snprintf` | same issue when appending |
| `sprintf(buf, ...)` | `snprintf(buf, size, ...)` | format output can exceed `buf` |

Spotting a call to `gets` or an unbounded `strcpy` is often the first thing you
look for when auditing C code or a disassembly.
