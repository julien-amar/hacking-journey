# Glossary

A quick reference for the acronyms and terms used across this repository. Each
entry links to the chapter where it's discussed in more depth.

## Architecture & execution

* **CPU register** — a small, very fast storage slot inside the CPU. See `0x04`.
* **RIP / EIP / PC** — *instruction pointer*: the address of the next instruction
  to run. Hijacking it is the goal of most control-flow exploits. See `0x04`.
* **RSP / ESP** — *stack pointer*: points to the top of the stack. See `0x04`.
* **RBP / EBP** — *base/frame pointer*: points to the base of the current stack
  frame. See `0x04`.
* **Stack** — LIFO memory region for per-function data (locals, saved registers,
  return addresses). See `0x04`, `0x0A`.
* **Heap** — memory region for dynamic, longer-lived allocations
  (`malloc`/`free`). See `0x04`, `0x0D`.
* **Endianness** — the byte order used to store multi-byte values. x86/x64 are
  *little-endian* (least-significant byte first). See `0x03`.
* **Calling convention** — the rules for how arguments are passed to functions
  (registers vs. stack). See `0x04`, `0x0B`.
* **Syscall** — a *system call*: the controlled entry point a user program uses
  to ask the kernel to do something privileged. See `0x08`.
* **MMU** — *Memory Management Unit*: hardware that maps virtual addresses to
  physical memory, giving each process its own address space. See `0x08`.
* **Protection ring** — CPU privilege level. Ring 3 = user apps, Ring 0 =
  kernel. See `0x08`.

## Binaries & linking

* **ELF** — *Executable and Linkable Format*: the binary format on Linux. See `0x06`.
* **PE** — *Portable Executable*: the binary format on Windows. See `0x06`.
* **Mach-O** — the binary format on macOS. See `0x06`.
* **Section** — a named region of a binary: `.text` (code), `.data`
  (initialised data), `.bss` (zero-initialised data), `.rodata` (constants). See `0x06`.
* **Symbol** — a named address (function or variable) recorded in the binary. See `0x06`.
* **GOT** — *Global Offset Table*: a table of pointers to external library
  functions, filled in at load/first-call time. See `0x0C`.
* **PLT** — *Procedure Linkage Table*: stub functions that route calls through
  the GOT to the dynamic linker. See `0x0C`.
* **Shared library** — reusable code loaded at runtime (`.so` on Linux, `.dll`
  on Windows, `.dylib` on macOS). See `0x0C`.
* **ASLR** — *Address Space Layout Randomization*: randomises memory base
  addresses each run so attackers can't hard-code them. See `0x0A`.
* **PIE** — *Position-Independent Executable*: a binary that can be loaded at a
  random base address (extends ASLR to the program itself). See `0x02`, `0x0A`.
* **NX / DEP** — *No-eXecute / Data Execution Prevention*: marks data memory
  (stack/heap) non-executable to block injected shellcode. See `0x0A`.
* **Stack canary** — a random value placed before the return address and checked
  before returning, to detect stack overflows. See `0x0A`.
* **RELRO** — *RELocation Read-Only*: makes the GOT read-only after startup to
  stop GOT-overwrite attacks. See `0x0A`, `0x0C`.

## Exploitation

* **Buffer overflow** — writing past the end of a buffer, corrupting adjacent
  memory. See `0x0A`, `0x0D`.
* **Shellcode** — small injectable machine code, classically spawning a shell. See `0x0A`.
* **NOP slide** — a run of "do nothing" instructions before shellcode so an
  imprecise jump still lands in it. See `0x0A`.
* **ret2libc** — reusing an existing library function (e.g. `system`) instead of
  injecting shellcode, to defeat NX. See `0x0A`.
* **ROP** — *Return-Oriented Programming*: chaining short existing code snippets
  ("gadgets") ending in `ret` to run arbitrary logic. See `0x0A`.
* **Gadget** — a short instruction sequence ending in `ret`, used in ROP. See `0x0A`.
* **Use-after-free (UAF)** — using memory through a pointer after it was freed. See `0x0D`.
* **unlink technique** — abusing the allocator's free-list pointer update into an
  arbitrary write. See `0x0D`.
* **Format-string bug** — passing user input as a `printf` format string,
  yielding arbitrary read (`%s`) and write (`%n`). See `0x11`.

## Analysis

* **Disassembler** — turns machine code back into assembly. See `0x06`.
* **Decompiler** — reconstructs higher-level C-like pseudocode from machine code. See `0x0F`.
* **Static analysis** — examining a program without running it. See `0x06`.
* **Dynamic analysis** — examining a program while it runs (debugger, tracing). See `0x05`, `0x06`.
* **Magic bytes** — a signature at the start of a file identifying its type. See `0x06`.
* **ptrace** — the Linux syscall debuggers use to control another process; also
  used for anti-debugging. See `0x07`.
* **Obfuscation / packing** — transforming code to resist analysis or to
  unpack itself only at runtime. See `0x07`, `0x0F`.

## Networking

* **TCP / IP** — reliable byte-stream transport / addressing & routing. See `0x0E`.
* **UDP** — connectionless, unreliable, lightweight transport. See `0x0E`.
* **Port** — a number identifying which program on a host should receive traffic. See `0x0E`.
* **Loopback** — the `lo` interface a host uses to talk to itself
  (`127.0.0.1` / `::1`). See `0x0E`.
* **Proxy** — a relay sitting between client and server, used here to inspect and
  tamper with traffic. See `0x0E`.
* **pcap** — a captured packet trace file (read with Wireshark / `tshark`). See `0x0E`, `0xFD`.

## Crypto

* **Encoding vs. encryption** — encoding (Base64/hex) is reversible without a
  key; encryption needs a key. See `0x10`.
* **Hash** — a one-way function; you can't "decrypt" it, only crack it. See `0x10`.
* **Symmetric cipher** — same key encrypts and decrypts (e.g. AES). See `0x10`.
* **Asymmetric cipher** — separate public/private keys (e.g. RSA). See `0x10`.
* **IV** — *Initialisation Vector*: randomises encryption so identical plaintext
  differs each time. See `0x10`.
* **ECB** — a block-cipher mode that leaks plaintext patterns. See `0x10`.
* **Salt** — random data added before hashing a password to defeat lookup tables. See `0x09`, `0x10`.

## Web

* **OWASP Top 10** — the canonical list of common web risk categories. See `0x12`.
* **SQLi** — *SQL injection*: user input alters a database query. See `0x12`.
* **XSS** — *Cross-Site Scripting*: attacker JS runs in another user's browser. See `0x12`.
* **CSRF** — *Cross-Site Request Forgery*: a victim's browser is tricked into a
  state-changing request. See `0x12`.
* **SSRF** — *Server-Side Request Forgery*: the server is tricked into fetching a
  URL for the attacker. See `0x12`.
* **IDOR** — *Insecure Direct Object Reference*: missing authorization lets you
  reach another user's object by changing an ID. See `0x12`.
* **LFI / path traversal** — using `../` to read files outside the intended
  directory. See `0x12`.
* **CSP** — *Content-Security-Policy*: an HTTP header that restricts what a page
  may load/run, mitigating XSS. See `0x12`.

## Platform

* **setuid / setgid** — a file bit that runs a program with the owner's/group's
  privileges; a common privilege-escalation target. See `0x09`.
* **UID / GID** — user / group ID numbers; UID 0 is root. See `0x09`.
* **APK / XAPK** — an Android app package / a bundle of split APKs. See `0xFF`.
* **adb** — *Android Debug Bridge*: the tool for talking to Android devices. See `0xFF`.
* **Frida** — a dynamic instrumentation toolkit for hooking running apps. See `0xFF`.
* **Certificate pinning** — an app rejecting any TLS cert but its expected one,
  blocking proxy interception. See `0xFF`.
