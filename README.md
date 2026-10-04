# hacking-journey

Want to learn hacking? This is a good place to start.

This repository is a collection of notes and knowledge I gathered while learning
about computer security, reverse engineering and binary exploitation. Each
chapter is a self-contained topic, ordered roughly from the fundamentals to more
advanced subjects.

> **Disclaimer.** Everything here is for educational purposes and for use in
> environments you own or are explicitly authorized to test (labs, CTFs, your
> own machines). Attacking systems without permission is illegal.

## Sources

The material comes from several places:

* [LiveOverflow's "Binary Exploitation / Memory Corruption" playlist](https://www.youtube.com/watch?v=iyAyN3GFM7A&list=PLhixgUqwRTjxglIswKp9mpkfPNfHkzyeN)
* CTF challenges
* Wikipedia / RFCs / official documentation / tutorials

## How the chapters are numbered

Chapters use hexadecimal numbers (`0x01`, `0x02`, … `0x10`, then `0xFD`…`0xFF`).
It is a small nod to the subject matter — and it keeps the folders sorted in the
right order. The low numbers build up the foundations; the `0xF*` numbers are
"appendix"-style topics that stand on their own.

## Table of contents

### Foundations

| # | Chapter | What you'll learn |
|---|---------|-------------------|
| `0x01` | [Terminal](0x01%20-%20Terminal/README.md) | Linux basics, essential shell commands, Bash, Vim |
| `0x02` | [Learning C](0x02%20-%20Learning%20C/README.md) | Writing and compiling your first C programs |
| `0x03` | [Learning Python](0x03%20-%20Learning%20Python/README.md) | Python for scripting exploits, number conversions, struct packing |
| `0x04` | [CPU](0x04%20-%20CPU/README.md) | Registers, execution flow, the stack and the heap |

### Analysis & debugging

| # | Chapter | What you'll learn |
|---|---------|-------------------|
| `0x05` | [Debugging](0x05%20-%20Debugging/README.md) | GDB and Radare2 commands, core dumps |
| `0x06` | [Reversing Binary](0x06%20-%20Reversing%20Binary/README.md) | Binary formats (ELF/PE/Mach-O), `file`, `strings`, `objdump`, tracing |
| `0x07` | [Anti-Debugging](0x07%20-%20Anti-Debugging/README.md) | How binaries try to resist analysis |
| `0x08` | [User mode vs Kernel mode](0x08%20-%20User%20mode%20vs%20Kernel%20mode/README.md) | Protection rings, syscalls, virtual memory |
| `0x09` | [User permissions](0x09%20-%20User%20permissions/README.md) | `/etc/passwd`, `/etc/shadow`, file permissions, setuid |

### Binary exploitation

| # | Chapter | What you'll learn |
|---|---------|-------------------|
| `0x0A` | [Stack overflow](0x0A%20-%20Stack%20overflow/README.md) | Overflowing buffers, controlling execution, ret2libc, ROP |
| `0x0B` | [x86 vs x64](0x0B%20-%20x86%20vs%20x64/README.md) | How C compiles differently on 32- vs 64-bit |
| `0x0C` | [Global Offset Table](0x0C%20-%20Global%20Offset%20Table/README.md) | GOT/PLT, dynamic loading, `LD_PRELOAD` hooking |
| `0x0D` | [Heap overflow](0x0D%20-%20Heap%20overflow/README.md) | Corrupting heap metadata and use-after-free |
| `0x11` | [Format String](0x11%20-%20Format%20String/README.md) | Arbitrary read/write via `printf(user_input)` and `%n` |

### Other topics

| # | Chapter | What you'll learn |
|---|---------|-------------------|
| `0x0E` | [Network Analyse](0x0E%20-%20Network%20Analyse/README.md) | TCP/IP, Wireshark, a hackable TCP/UDP proxy |
| `0x0F` | [Decompilers](0x0F%20-%20Decompilers/README.md) | Tools to turn binaries back into readable code |
| `0x10` | [Cryptanalysis](0x10%20-%20Cryptanalysis/README.md) | Encoding/decoding, hashing, password cracking, why crypto breaks |
| `0x12` | [Web Security](0x12%20-%20Web%20Security/README.md) | SQLi, XSS, SSRF, IDOR and the other OWASP classes |
| `0xFD` | [Forensic](0xFD%20-%20Forensic/README.md) | Memory/disk dumps, file carving, metadata, stego |
| `0xFE` | [Process Threads](0xFE%20-%20Process%20Threads/README.md) | The `/proc` pseudo-filesystem |
| `0xFF` | [Android](0xFF%20-%20android/README.md) | Decompiling APKs, Frida hooking, SSL pinning |

## Suggested learning path

If you are starting from scratch, a good order is:

1. Get comfortable in the shell (`0x01`) and write small programs (`0x02`, `0x03`).
2. Understand what the CPU is actually doing (`0x04`), then learn to watch it
   with a debugger (`0x05`).
3. Learn to read a compiled binary (`0x06`, `0x0B`).
4. Put it together with your first real exploits (`0x0A` → `0x11` → `0x0C` → `0x0D`).

Web hacking (`0x12`) is a largely independent track — you can start it any time,
and it needs less low-level background than the binary-exploitation chapters.

## Where to practice

Hands-on practice matters more than reading. Some good platforms:

* [exploit.education](https://exploit.education/) — the Phoenix VM used in the exploitation chapters
* [pwn.college](https://pwn.college/) — structured, university-style curriculum
* [OverTheWire](https://overthewire.org/wargames/) — start with *Bandit* then *Narnia*
* [picoCTF](https://picoctf.org/) — beginner-friendly CTF with a permanent practice set
* [Root-Me](https://www.root-me.org/) — large catalogue of challenges by category
* [microcorruption](https://microcorruption.com/) — embedded/assembly puzzles in the browser
* [Hack The Box](https://www.hackthebox.com/) & [TryHackMe](https://tryhackme.com/) — full target machines

## TODO / to explore further

Root-Me challenge categories still to work through:

* <https://www.root-me.org/fr/Challenges/Programmation/>
* <https://www.root-me.org/fr/Challenges/Realiste/>
* <https://www.root-me.org/fr/Challenges/Reseau/>
* <https://www.root-me.org/fr/Challenges/Steganographie/> (basics now covered in `0xFD`)
* <https://www.root-me.org/fr/Challenges/Web-Client/> (see `0x12`)
* <https://www.root-me.org/fr/Challenges/Web-Serveur/> (see `0x12`)

Other ideas to expand:

* Revisit and expand chapter `0x0E` (Network Analyse).
* A dedicated **shellcoding** chapter (currently summarised inside `0x0A`).
* Hands-on **crypto** challenges (Cryptopals), building on `0x10`.
