# Reversing binary

Each operating system (Linux, Windows, Mac)  have it's own standard for structuring binaries.

Linux: https://en.wikipedia.org/wiki/Executable_and_Linkable_Format (ELF, Executable and Linkable Format)
Windows: https://en.wikipedia.org/wiki/Portable_Executable (PE, Portable Executable)
Mac: https://en.wikipedia.org/wiki/Mach-O (Mach object)

## file

`file <file>` enable to determine a speicifc file type (based on magic table).

Indexed magic database is defined in:
* `/etc/magic` (local definition)
* in the binary file `/usr/share/misc/magic.mgc`
* spread over multiple fragment in `/usr/share/misc/magic`

_For more details: https://www.systutorials.com/docs/linux/man/5-magic/_

## strings

`strings <file>` prints the sequences of printable characters in specified files.
It enables to identify non encrypted strings that are stored in the binary.

## Dumpbin (PE files)

Microsoft provide a tool to analyse PE file structure, named `dumpbin`.

_Documentation: https://docs.microsoft.com/en-us/cpp/build/reference/dumpbin-reference?view=vs-2019_

## objdump (PE or ELF files)

Objdump enables to retrieve a lot of information from a binary file (headers, debugging informations, symbols, ...)

`objdump -d <file>` enables to disassemble a specific binary.
`objdump -x <file>` enables to display all available header information, including the symbol table and relocation entries.

_For more details: https://www.systutorials.com/docs/linux/man/1-objdump/_

### Headers

Program headers information, enable to identify if Stack informations are executable. (this can be a suitable information for a possible buffer overflow exploit)
```
STACK off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**4
         filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
```

### Sections

Each part of our binary is spread among several sections:
* `.text` section holds the application code
* `.rodata` section holds our readonly data, such as binary's strings

## strace, ltrace, ftrace, ktrace

Linux system provides several kinds of functions called `syscalls` (for system calls).

_For more details: https://www.systutorials.com/docs/linux/man/2-syscalls/_

### strace (syscall)

`strace` enables to trace syscall & signals, showing passed arguments.
This command might not be available by default, you can install the `strace` package on Kali.

_For more details: https://www.systutorials.com/docs/linux/man/1-strace/_

### ltrace (library)

`ltrace` enables to trace external library functions calls, showing passwed arguments.
This command might not be available by default, you can install the `ltrace` package on Kali.

_For more details: https://www.systutorials.com/docs/linux/man/1-ltrace/_

### ftrace (function)

`ftrace` is similar to `strace`, it will additionaly trace function calls.
This command might not be available in all systems.

_For more details: https://www.systutorials.com/docs/linux/man/1-ftrace/_

### ktrace (Kernel)

`ktrace` enables to trace kernel processing.
This command might not be available in all systems.

_For more details: https://www.freebsd.org/cgi/man.cgi?ktrace(1)_
