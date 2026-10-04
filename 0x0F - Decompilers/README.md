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
