# User mode vs Kernel mode

**In short:** the CPU runs code at different privilege levels. Your normal
programs run in *user mode* (restricted — they cannot touch hardware or other
processes directly). The operating-system kernel runs in *kernel mode* (full
control of the machine). The only sanctioned way for a user-mode program to ask
the kernel to do something privileged (open a file, create a process, send a
packet) is a **system call**. This boundary is central to security: a huge
amount of exploitation is about tricking privileged code into doing something on
a less-privileged attacker's behalf.

```
         Ring 3  user applications        (least privileged)
         Ring 2  }
         Ring 1  }  rarely used on modern general-purpose OSes
         Ring 0  operating-system kernel   (most privileged)
        Ring -1  hypervisor (VT-x / AMD-V)
```

## Protection rings

```
In computer science, hierarchical protection domains, often called protection rings, are mechanisms to protect data and functionality from faults (by improving fault tolerance) and malicious behavior (by providing computer security). This approach is diametrically opposite to that of capability-based security.

On most operating systems, Ring 0 is the level with the most privileges and interacts most directly with the physical hardware such as the CPU and memory.

Special gates between rings are provided to allow an outer ring to access an inner ring's resources in a predefined manner, as opposed to allowing arbitrary usage. Correctly gating access between rings can improve security by preventing programs from one ring or privilege level from misusing resources intended for programs in another.
```

_Source: https://en.wikipedia.org/wiki/Protection_ring_

## Supervisor mode

```
In computer terms, supervisor mode is a hardware-mediated flag which can be changed by code running in system-level software. System-level tasks or threads will have this flag set while they are running, whereas userspace applications will not. This flag determines whether it would be possible to execute machine code operations such as modifying registers for various descriptor tables, or performing operations such as disabling interrupts. The idea of having two different modes to operate in comes from "with more control comes more responsibility" – a program in supervisor mode is trusted never to fail, since a failure may cause the whole computer system to crash.

Linux, macOS and Windows are three operating systems that use supervisor/user mode.

To perform specialized functions, user mode code must perform a system call into supervisor mode or even to the kernel space where trusted code of the operating system will perform the needed task and return the execution back to the userspace. Additional code can be added into kernel space through the use of loadable kernel modules, but only by a user with the requisite permissions, as this code is not subject to the access control and safety limitations of user mode.
```

_Source: https://en.wikipedia.org/wiki/Protection_ring_

### Syscalls

Syscalls are triggered through a `syscall` instruction (on x86-64). Before it,
the program puts the **syscall number** in `rax` and the arguments in the usual
argument registers — `rdi, rsi, rdx, r10, r8, r9` (note: `r10`, *not* `rcx` as
in a normal function call, because `syscall` clobbers `rcx`). The return value
comes back in `rax`.

When the kernel needs to move data across the boundary, it does so with
`copy_from_user` / `copy_to_user` rather than touching user pointers directly —
precisely so a malicious user-space address can't trick the kernel into reading
or writing memory it shouldn't.

#### Hands-on: "hello" with raw syscalls

This writes to stdout (`write`, syscall `1`) then exits (`exit`, syscall `60`) —
no libc involved. Save as `hello.s`:

```asm
section .data
msg:    db  "hello via syscall", 10      ; 10 = newline
len     equ $ - msg

section .text
global _start
_start:
    mov rax, 1          ; syscall number: write
    mov rdi, 1          ; fd 1 = stdout
    mov rsi, msg        ; buffer
    mov rdx, len        ; length
    syscall

    mov rax, 60         ; syscall number: exit
    mov rdi, 0          ; exit status 0
    syscall
```

Assemble, link and run:

```sh
nasm -f elf64 hello.s -o hello.o
ld hello.o -o hello
./hello                 # -> hello via syscall
```

#### Seeing it from the outside

`strace` (chapter `0x06`) shows the same two syscalls the program makes:

```sh
$ strace ./hello
write(1, "hello via syscall\n", 18)     = 18
exit(0)                                  = ?
```

This is the whole user/kernel story in miniature: user code prepares arguments,
executes `syscall` to cross into the kernel, the kernel does the privileged work
and returns. The numbers differ per architecture — look them up here:

#### Finding syscall numbers

* x86-64 table (number ↔ name ↔ arguments): https://filippo.io/linux-syscall-table/
* On a running system: `ausyscall --dump`, or read
  `/usr/include/asm/unistd_64.h`.
* `man 2 <name>` documents each one (e.g. `man 2 write`).

_For more details: https://man7.org/linux/man-pages/man2/syscalls.2.html_

## Hypervisor mode

```
Recent CPUs from Intel and AMD offer x86 virtualization instructions for a hypervisor to control Ring 0 hardware access. Although they are mutually incompatible, both Intel VT-x (codenamed "Vanderpool") and AMD-V (codenamed "Pacifica") create a new "Ring -1" so that a guest operating system can run Ring 0 operations natively without affecting other guests or the host OS.
```

_Source: https://en.wikipedia.org/wiki/Protection_ring_

## Memory management

Memory addresses are mapped (by the Kernel) in the memory management unit (MMU) to provide processes a linear virtual addressable space.


A page fault error (illegal memory access) is raised when a program try to access a memory page that is not mapped to the current process.

_For more details: https://en.wikipedia.org/wiki/Memory_management_unit_

## References

* Linux device drivers: https://lwn.net/Kernel/LDD3/
