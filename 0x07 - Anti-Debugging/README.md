# Anti Debugging techniques

Debugger, decompilers & binary loaders implement their own version of binary parser to analyze/execute it.  
It means that, there might be some way of preventing a specific binary to be loaded/debugged/analyzed by altering its structure.

_Presentation: https://www.blackhat.com/docs/us-14/materials/arsenal/us-14-Hernandez-Melkor-Slides.pdf_

## Structure alteration

Possible ways of preventing a binary of being analyzed is basically to generate invalid assembly code or non-existant instructions.

Additionnal malformed sections could be added to a binary to prevent it from being analyzed.
_Source: https://github.com/IOActive/FileFormatFuzzing/blob/master/ELFAntiDebuggingTools/gdb_751_elf_shield.c_

Fuzzing the binary content, by altering a single byte, could also prevent some tool to load the program.
_Source: https://github.com/LiveOverflow/liveoverflow_youtube/blob/master/0x07_0x08_uncrackable_crackme/fuzz.py_

## Runtime technique

Debugger detection could also be checked at run time, using `ptrace` syscall:

```c
if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1){
    printf("Debugging is not allowed!\n");
    exit(-1);
}
```
