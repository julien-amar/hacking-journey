# Reversing binary

*Reversing* a binary means working backwards from a compiled program to
understand what it does, without having the source code. The first steps are
always the cheapest: identify the file type, pull out readable strings, and look
at the headers and sections before reaching for a disassembler.

Each operating system (Linux, Windows, macOS) has its own standard for
structuring executable files:

* Linux: https://en.wikipedia.org/wiki/Executable_and_Linkable_Format (ELF, Executable and Linkable Format)
* Windows: https://en.wikipedia.org/wiki/Portable_Executable (PE, Portable Executable)
* macOS: https://en.wikipedia.org/wiki/Mach-O (Mach-O, Mach object)

## file

On Windows, the registry database links a file extension to the software that
can open it.

On other systems, the extension is just a hint; `file <file>` determines the
real type by inspecting the file's *magic bytes* (a signature near the start of
the file).

The indexed magic database is available at these locations:
* `/etc/magic` (local definition)
* in a binary format `/usr/share/misc/magic.mgc`
* spread over multiple fragment in `/usr/share/misc/magic`

_For more details: https://man7.org/linux/man-pages/man4/magic.4.html_

## strings

`strings <file>` prints the sequences of printable characters in specified files.
It enables to identify non encrypted strings that are stored in the binary.

## Dumpbin (PE files)

Microsoft provide a tool to analyse PE file structure, named `dumpbin`.

_Documentation: https://docs.microsoft.com/en-us/cpp/build/reference/dumpbin-reference?view=vs-2019_

## objdump (PE or ELF files)

`objdump` retrieves information from a binary file (headers, debugging
information, symbols, …).

* `objdump -d <file>` — disassemble the executable sections.
* `objdump -M intel -d <file>` — same, but in the (often easier to read) Intel
  syntax instead of the default AT&T.
* `objdump -x <file>` — display all header information, including the symbol
  table and relocation entries.

_For more details: https://www.man7.org/linux/man-pages/man1/objdump.1.html_

> Related: `readelf -a <file>` and `nm <file>` are also handy for ELF headers
> and symbol tables respectively. To quickly check which exploit mitigations a
> binary was built with (NX, stack canary, PIE, RELRO), use
> [`checksec`](https://github.com/slimm609/checksec.sh) — `checksec --file=<bin>`.

### Headers

Binary headers describe the file's layout and capabilities — for example,
whether the stack is marked as an executable memory segment, which tells you
whether a plain stack-based shellcode exploit is even possible:

```
STACK off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**4
         filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
```

### Sections

On Linux, binaries are split into several sections, among them:
* `.text` holds the application code (the instructions)
* `.data` holds initialised, mutable global data
* `.bss` holds uninitialised global data (zeroed at startup)
* `.rodata` holds read-only data, such as the binary's string literals

_For more details on Linux ELFsections: https://www.intezer.com/blog/research/executable-linkable-format-101-part1-sections-segments/_

_For more details on Windows PE sections: https://docs.microsoft.com/en-us/windows/win32/debug/pe-format#special-sections_

## Dynamic analysis: strace, ltrace, ftrace, ktrace

The tools above are *static* (they inspect the file at rest). The `*trace` tools
are *dynamic*: they run the program and report what it does. Watching the
syscalls and library calls a program makes often reveals its logic faster than
reading the disassembly — e.g. which files it opens, what it reads, what it
compares your input against.

To let a program interact with the Linux kernel (for system-wide capabilities
like opening files or creating processes), the kernel exposes special entry
points called `syscalls` (system calls).

_For more details, see chapter `0x08 - User mode vs Kernel mode`_

### strace (syscall)

`strace` enables to trace syscall & signals, showing passed arguments.  
This command might not be available by default, you can install the `strace` package on Kali.  

_For more details: https://man7.org/linux/man-pages/man1/strace.1.html_

### ltrace (library)

`ltrace` enables to trace external library functions calls, showing passed arguments.  
This command might not be available by default, you can install the `ltrace` package on Kali.  

_For more details: https://man7.org/linux/man-pages/man1/ltrace.1.html_

### ftrace (function)

`ftrace` is similar to `strace`, it will additionaly trace function calls.  
This command might not be available in all systems.  

_For more details: https://linux.die.net/man/1/ftrace_

### ktrace (Kernel)

`ktrace` enables to trace kernel level interactions.  
This command might not be available in all systems.  

_For more details: https://www.freebsd.org/cgi/man.cgi?ktrace(1)_
