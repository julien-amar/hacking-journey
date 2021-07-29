# Reversing binary

Each operating system (Linux, Windows, Mac)  have it's own standard for structuring binaries.

* Linux: https://en.wikipedia.org/wiki/Executable_and_Linkable_Format (ELF, Executable and Linkable Format)  
* Windows: https://en.wikipedia.org/wiki/Portable_Executable (PE, Portable Executable)  
* Mac: https://en.wikipedia.org/wiki/Mach-O (Mach object)  

## file

In Windows, the registry database link file extension with compatible softwares.

On other systems, `file <file>` enable to determine a specific file type (based on magic table).

The indexed magic database is accessible at those locations:
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

Objdump enables to retrieve information from a binary file (headers, debugging informations, symbols, ...)

`objdump -d <file>` enables to disassemble a specific binary.
`objdump -x <file>` enables to display all available header information, including the symbol table and relocation entries.

_For more details: https://www.man7.org/linux/man-pages/man1/objdump.1.html_

### Headers

Binary headers enable to define supported capabilities (by example: is Stack declared as an executable memory segment; suitable information for a possible buffer overflow exploit)

```
STACK off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**4
         filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
```

### Sections

On Linux, binaries are splitted over several sections, among them:
* `.text` section holds the application code
* `.data` section holds our mutable data.
* `.rodata` section holds our readonly data, such as binary's strings

_For more details on Linux ELFsections: https://www.intezer.com/blog/research/executable-linkable-format-101-part1-sections-segments/_

_For more details on Windows PE sections: https://docs.microsoft.com/en-us/windows/win32/debug/pe-format#special-sections_

## strace, ltrace, ftrace, ktrace

To enable the interaction between a program & the Linux Kernel (to provide system wide capabilities), special functions are provided. They are called `syscalls` (for system calls).

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
