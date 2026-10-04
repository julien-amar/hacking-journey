# Decompilers

A **compiler** transforms human-readable source code into machine code (or
bytecode) that the CPU or a virtual machine can execute. Tools exist to reverse
parts of this process:

* A **disassembler** turns machine code back into assembly instructions (a
  faithful, low-level view — e.g. `objdump`, chapter `0x06`).
* A **decompiler** goes further and tries to reconstruct higher-level,
  C-like *pseudocode*. It is not a perfect round-trip (comments, variable names
  and exact structure are lost at compile time), but it is far easier to read.

To make this harder, developers can **obfuscate** or **pack** their code so the
output is confusing or only unpacks itself at runtime.

## Binary (ASM)

### Decompilers

| Name          | Capabilities       | Notes | Link                                       |
| ------------- | ------------------ | ----- | ------------------------------------------ |
| **Ghidra**    | Decompile          | Free & open-source (by the NSA); excellent decompiler — best starting point | https://ghidra-sre.org/ |
| IDA           | Decompile / Debug  | Industry standard; the decompiler (Hex-Rays) is a paid add-on; free edition available | https://www.hex-rays.com/products/idahome/ |
| Hopper        | Decompile          | Affordable, popular on macOS | https://www.hopperapp.com/                 |
| Radare2       | Decompile / Debug  | CLI-driven, scriptable, free | https://rada.re/n/                         |
| Cutter        | Decompile          | Free GUI front-end for Radare2 | https://cutter.re/                         |
| Binary Ninja  | Decompile          | Commercial; strong analysis API | https://binary.ninja/                      |

### A typical workflow (Ghidra)

Tools differ in the details, but reversing a native binary almost always follows
the same loop. Using Ghidra as the example:

1. **Create a project and import the binary** (`File → Import File`), then
   double-click it to open the CodeBrowser.
2. **Let it auto-analyze** (accept the defaults the first time). This finds
   functions, strings, cross-references and recovers the pseudocode.
3. **Start from something you recognise.** Good entry points:
   * the **Symbol Tree → Functions → `main`**, or
   * the **Defined Strings** window (`Window → Defined Strings`) — find a prompt
     like `"Enter password:"`, then right-click → *References → Show References
     to Address* to jump to the code that uses it.
4. **Read the Decompiler pane** (right-hand side) rather than the raw assembly —
   it shows C-like pseudocode for the currently selected function.
5. **Make it readable.** Rename variables and functions (`L` on a symbol) and
   retype things as you understand them; the decompiler updates live. Add
   comments (`;`).
6. **Follow the logic** of the check or algorithm you care about (e.g. how input
   is compared, where a buffer is filled), cross-referencing with a debugger
   (chapter `0x05`) when you need to see real runtime values.

> The decompiled C is a *reconstruction*, not the original source — expect
> auto-generated names (`local_18`, `FUN_00401234`), occasional wrong types, and
> no comments. Your job is to add that meaning back as you read.

This same "find a string → find its xref → read the function" loop is the single
most useful habit for crackmes and reversing CTF challenges.

## .NET

### Decompilers

| Name          | Capabilities       | Link                                             |
| ------------- | ------------------ | ------------------------------------------------ |
| dnSpy         | Decompile / Debug  | https://github.com/dnSpy/dnSpy                   |
| JustDecompile | Decompile          | https://www.telerik.com/products/decompiler.aspx |
| dotPeek       | Decompile          | https://www.jetbrains.com/decompiler/            |

### De-Obfuscators / Unpackers

| Name          | Capabilities           | Link                                                        |
| ------------- | ---------------------- | ----------------------------------------------------------- |
| de4dot        | Deobfuscator /Unpacker | https://github.com/de4dot/de4dot                            |
| JustDecompile | Modular                | https://www.telerik.com/products/decompiler/extensions.aspx |

_More links available here: https://github.com/NotPrab/.NET-Deobfuscator_
