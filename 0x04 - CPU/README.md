# CPU

To exploit (or just understand) a program you need a mental model of what the
CPU does: it reads instructions one at a time, keeps a small set of fast
variables called registers, and uses memory (the stack and the heap) for
everything that doesn't fit.

## Registers

Depending on the architecture, a CPU has a certain number of registers. You can
think of registers as a handful of very fast global variables that the CPU
works with directly.

The *bitness* of the machine (32- or 64-bit) sets the width of these registers.

Many register names refer to *subparts* of a larger register. On x86-64, `RAX`
is 64 bits; `EAX` is its low 32 bits; `AX` the low 16; `AL`/`AH` the low two
bytes. Writing to `EAX` therefore also changes `RAX`:

```
|__64__|__56__|__48__|__40__|__32__|__24__|__16__|__8___|
|__________________________RAX__________________________|
|xxxxxxxxxxxxxxxxxxxxxxxxxxx|____________EAX____________|
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx|_____AX______|
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx|__AH__|__AL__|
```

Among the registers, some are special:

* The **program counter** / **instruction pointer** (`RIP` on x86-64, `EIP` on
  x86, `PC` on ARM) holds the address of the next instruction to execute.
  Hijacking this register is the goal of most control-flow exploits.
* The **stack pointer** (`RSP`/`ESP`) points to the top of the stack.
* The **base pointer** / frame pointer (`RBP`/`EBP`) points to the base of the
  current stack frame.
* The **flags register** (`RFLAGS`/`EFLAGS`) holds status bits set as a side
  effect of arithmetic/comparison instructions. For example, the **zero flag
  (ZF)** is set to `1` when the result of the last operation was exactly zero
  (e.g. when `cmp a, b` finds `a == b`). Conditional jumps like `je`/`jne` act
  on these flags.

## Execution flow

By default the CPU executes instructions in sequence. The flow is changed by
"go to"-style instructions:

* jumps: `jmp` (unconditional), `je`/`jne`/`jg`/`jl`… (conditional, based on the
  flags register)
* branch: `bne`, `beq` (on architectures like ARM/MIPS)
* call: `call` (jump, but remember where to come back to)

### Jump

A jump is essentially "set `RIP` to this address". A conditional jump does the
same only if a given flag is set.

### Call and return

A function can be called from many places, so the CPU must remember *where to
return* each time. A `call` pushes the address of the next instruction (the
*return address*) onto the stack, then jumps to the function.

At the end of the function, `leave` + `ret` tear down the frame and return:

* `leave` restores the caller's frame (roughly `mov rsp, rbp; pop rbp`).
* `ret` pops the saved return address off the stack into `RIP`, resuming at the
  caller.

By convention the return value is left in `RAX`/`EAX`.

> Because the return address lives *on the stack*, a buffer overflow that reaches
> it can overwrite where the function returns to — this is the core idea behind
> chapter `0x0A`.

### Calling conventions

How arguments are passed to a function depends on the architecture's calling
convention:

* **x86-64 (System V, used by Linux/macOS):** the first six integer/pointer
  arguments go in registers `RDI, RSI, RDX, RCX, R8, R9`; any extras go on the
  stack; the return value comes back in `RAX`.
* **x86 (32-bit, cdecl):** all arguments are pushed onto the stack (right to
  left); the return value comes back in `EAX`.

Knowing this tells you where to put values when you hijack a call (e.g. putting
the address of `"/bin/sh"` into `RDI` before calling `system`).

## Memory management

There is only a small number of registers, so anything that doesn't fit lives in
memory — on the **heap** or the **stack**:

```
+-------------+ <- high addresses (e.g. 0xffffffff)
|             |
|    STACK    |  grows downward (toward lower addresses)
|             |
+vvvvvvvvvvvvv+
|             |
|   (unused)  |
|             |
+^^^^^^^^^^^^^+
|             |
|    HEAP     |  grows upward (toward higher addresses)
|             |
+-------------+
|             |
|   PROGRAM   |  code (.text), then data (.data/.bss)
|             |
+-------------+ <- low addresses (e.g. 0x00000000)
```

## Heap

The heap is a region used to store data that must live beyond a single function
call (long-term, dynamically sized allocations — `malloc`/`free` in C).

You read and write memory with `mov`:

```asm
; Load the value stored at address 0x14 into EAX
mov eax, [0x14]
```

(The brackets mean "the value *at* this address", i.e. a dereference.)

## Stack

The stack stores short-lived, per-function context: local variables, saved
registers, and return addresses. The top of the stack is tracked by
`SP`/`ESP`/`RSP`.

You push and pop with:

```asm
push 0x05   ; put 0x05 on top of the stack; RSP decreases (stack grows down)
pop  eax    ; take the top value into EAX;  RSP increases
```

The stack is made of **frames** — one per active function call. Each frame
records enough to restore the previous one, forming the *call stack*:

```
+-----------------+
|    PREVIOUS     |
|  STACK  FRAME   |
+-----------------+
| RETURN  ADDRESS |   <- where ret will jump back to (overwrite target in 0x0A)
+-----------------+ <- current RBP (base of the current frame)
|  SAVED  RBP     |   <- the caller's RBP, restored by leave
+-----------------+
|                 |
|   LOCAL DATA    |   <- local variables, buffers
|                 |
+-----------------+ <- current RSP (top of the current frame)
```

## Resources

Practice and read more here:

* <https://microcorruption.com/> (demo account: `matasano` / password: `matasano`)
* <https://www.recurse.com/blog/7-understanding-c-by-learning-assembly>
* [Godbolt Compiler Explorer](https://godbolt.org/) — type C, see the assembly
  it produces instantly (great for the lessons in chapter `0x0B`).
