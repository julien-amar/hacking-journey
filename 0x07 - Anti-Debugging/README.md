# Anti-debugging techniques

Debuggers, decompilers and binary loaders each implement their own parser to
analyse or execute a binary. This means a program can try to *resist* analysis —
either by malforming its own structure so the tools choke on it, or by detecting
at runtime that it is being watched and changing its behaviour.

As an analyst, the point of this chapter is twofold: recognise these tricks when
you hit them, and know that they are speed bumps, not walls — they can almost
always be patched out or bypassed.

_Presentation: https://www.blackhat.com/docs/us-14/materials/arsenal/us-14-Hernandez-Melkor-Slides.pdf_

## Structure alteration (anti-analysis)

These techniques target the *tools*, not the running program:

* **Invalid / non-existent instructions.** Emitting bytes that decode to garbage
  can confuse a linear-sweep disassembler into misaligning all following
  instructions.
* **Malformed sections / headers.** Adding extra or deliberately broken sections
  can make a loader refuse the file while the OS still runs it.
  _Example: https://github.com/IOActive/FileFormatFuzzing/blob/master/ELFAntiDebuggingTools/gdb_751_elf_shield.c_
* **Fuzzing the binary.** Flipping a single byte can be enough to break a
  specific tool's parser while leaving execution intact.
  _Example: https://github.com/LiveOverflow/liveoverflow_youtube/blob/master/0x07_0x08_uncrackable_crackme/fuzz.py_

## Runtime techniques (anti-debugging)

These detect a debugger while the program is running.

### ptrace self-attach

On Linux a process can only be traced by one tracer at a time. A program can
call `ptrace(PTRACE_TRACEME, …)` on itself: if it succeeds, no debugger is
attached; if it fails, something is already tracing it.

```c
#include <sys/ptrace.h>
#include <stdio.h>
#include <stdlib.h>

if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1) {
    printf("Debugging is not allowed!\n");
    exit(-1);
}
```

### Other common checks

* **`/proc/self/status`** — the `TracerPid` field is non-zero when a debugger is
  attached. A program can read this file and bail out.
* **Timing checks** — single-stepping in a debugger is slow, so a program can
  measure the time between two points (e.g. with `rdtsc`) and assume a debugger
  if it is suspiciously long.
* **Breakpoint detection** — software breakpoints work by overwriting an
  instruction with the `0xCC` byte (`int3`); a program can scan its own `.text`
  for `0xCC` to spot them.

## Bypassing them

Because these are just conditional checks in the code, the usual responses are:

* **Patch the check.** Find the comparison/branch in the disassembly and NOP it
  out or invert it (e.g. with your debugger, Ghidra's patch feature, or a hex
  editor).
* **Lie to the check.** Hook or intercept the relevant call so it returns the
  "not being debugged" answer — for example with `LD_PRELOAD` (see chapter
  `0x0C`) to override `ptrace`, or a debugger script that forces the return
  value.
* **Set a breakpoint before the check** and simply edit the register holding its
  result.
