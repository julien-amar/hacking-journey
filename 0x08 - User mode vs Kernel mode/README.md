# User mode vs Kernel mode

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

Syscalls are triggered through a `syscall` instruction. This instruction is always prefixed by a `mov eax, <code>` that indicates to the kernel what needs to be processed.

When the Kernel requires to access some data from/to the user land, it will copy that data from/to the user land using `copy_from_user` or `copy_to_user` methods.

_For more details: https://man7.org/linux/man-pages/man2/syscalls.2.html & https://filippo.io/linux-syscall-table/_

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
