# Global Offset Table (GOT)

Reusable code can be extracted into a shared **library** (`*.so` on Linux,
`*.dll` on Windows) so that many programs share one copy instead of each
bundling its own — this keeps binaries smaller.

The required libraries are loaded when the process starts. But the program still
needs to know *at which address* each library function ended up — and with ASLR
enabled (see chapter `0x0A`), that address is different every run, so it can't
be hard-coded at compile time.

The **GOT** solves this: it is a table of pointers to external functions. When
code calls, say, `printf`, it actually jumps through the GOT entry for `printf`.

By default the GOT is writable, because the real addresses are resolved lazily
by the dynamic loader (`ld.so`) on each function's *first* call, and written
into the table at that point.

> **Why attackers care.** Because the GOT is a table of function pointers that
> the program jumps through, overwriting a GOT entry (e.g. via a heap overflow
> or a format-string bug) redirects a later call to an address of your choosing
> — see the Heap One example in chapter `0x0D`. The **Full RELRO** mitigation
> resolves everything at startup and then makes the GOT read-only to block this.

Documentation: https://ctf101.org/binary-exploitation/what-is-the-got/
Documentation: https://ctf101.org/binary-exploitation/relocation-read-only/

# Procedure Linkage Table (PLT)

To do the resolution, some trampoline functions (PLT) are injected in the process memory, to interface with the GOT, those functions are responsible for calling the dynamic linker.

# Dynamic loading

It is possible to override some library or methods, by configuring the loader, through:
* Environment variable `LD_LIBRARY_PATH` or `LD_PRELOAD`. _(Except if the executable is a setuid/setgid binary, in which case it is ignored.)_
* From the cache file `/etc/ld.so.cache` which contains a compiled list of candidate libraries previously found in the augmented library path.
* In the default path `/lib`, and then `/usr/lib`.

For more details: https://manpages.ubuntu.com/manpages/precise/man8/ld.so.8.html

## LD_PRELOAD

Through this section, we will describe how to override an external function by your own.

We will use following program (`main.c`), generating random numbers:

```c
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  srand(time(NULL));
  int x = rand();
  printf("%d\n", x);
  return 0;
}
```

Compile the program and execute it:

```sh
$ gcc -Wall main.c -o program
$ ./program 
1080548345       
$ ./program 
1540100247 
```

`rand` is a `libc` function (located in `/lib`), that returns random numbers:

```sh
$ ldd ./program 
  linux-vdso.so.1 (0x00007ffffa442000)
  libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007ff483910000)
  /lib64/ld-linux-x86-64.so.2 (0x00007ff483af7000)
```

We will create a library (`override.c`) that will override the `rand` function of the `libc`:

```c
#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>

typedef int (*rand_t)(void);

int rand(void)
{
  rand_t base = (rand_t)dlsym(RTLD_NEXT, "rand");
  int x = base();
  int returned_value = 42;
  printf("Returning %d instead of %d\n", returned_value, x);
  return returned_value;
}
```

Compile this implementation as a library and execute the intial program with it:

```sh
$ gcc -shared -fPIC -Wall override.c -o override.so -ldl
$ LD_PRELOAD=./override.so ./program
Returning 42 instead of 1560828960
42
$ LD_PRELOAD=./override.so ./program
Returning 42 instead of 1260732614
42
```

For your information, the paramter `-fPIC` _(Position Independent Code)_, means that the generated machine code is not dependent on being located at a specific address in order to work.
