# Setting up your lab

You need a safe, disposable Linux environment to work through these chapters.
**Do your experimenting in a VM or container, not on your main machine** — you
will be disabling security features, running untrusted binaries, and
occasionally crashing things.

> Only ever attack targets you own or are explicitly authorised to test.

## Option A — a Linux VM (recommended to start)

A virtual machine gives you a full, isolated system you can snapshot and roll
back.

1. Install a hypervisor: [VirtualBox](https://www.virtualbox.org/) (free) or
   VMware.
2. Get a Linux image:
   * **[Kali Linux](https://www.kali.org/get-kali/)** — ships with most of the
     offensive tools in these chapters pre-installed (gdb, radare2, binwalk,
     john, hashcat, wireshark, the `wordlists`…). Easiest if you don't want to
     install things one by one.
   * **Ubuntu/Debian** — a clean general-purpose choice; you install tools as
     each chapter needs them.
3. **Take a snapshot** of the fresh install so you can always return to a known-
   good state.

> Apple Silicon (M1/M2…) Macs run ARM VMs, not x86 — fine for most chapters, but
> the x86/x64 exploitation examples assume an x86-64 guest. Use UTM/QEMU with
> an x86-64 image, or run those parts on an x86 machine/cloud VM.

## Option B — Docker (lightweight, per-task)

Good when you just want a throwaway Linux shell on an existing machine:

```sh
# A disposable Ubuntu container with your current folder mounted in
docker run --rm -it -v "$PWD":/work -w /work ubuntu:22.04 bash

# Some exploitation tasks need relaxed protections; this disables ASLR
# inside the container (see chapter 0x0A):
docker run --rm -it --privileged ubuntu:22.04 bash
```

Note: containers share the host kernel, so kernel-level topics (`0x08`) and some
`ptrace`/debugging behaviour are better explored in a real VM.

## Core toolkit by chapter

On Ubuntu/Debian you can install most of what the repo uses with:

```sh
sudo apt-get update
sudo apt-get install -y \
    build-essential gcc gcc-multilib gdb \
    binutils file strace ltrace \
    python3 python3-pip \
    radare2 binwalk exiftool sleuthkit \
    nasm netcat-openbsd nmap tcpdump wireshark \
    john hashcat

pip3 install pwntools
```

| Chapter | Tools |
|---------|-------|
| `0x02` C | `gcc`, `gcc-multilib` (for 32-bit builds in `0x0B`) |
| `0x03` Python | `python3`, `pip install pwntools` |
| `0x05` Debugging | `gdb` + a plugin ([pwndbg](https://github.com/pwndbg/pwndbg)), optional `radare2` |
| `0x06` Reversing | `binutils` (`objdump`, `readelf`, `nm`), `file`, `strace`, `ltrace`, [`checksec`](https://github.com/slimm609/checksec.sh) |
| `0x08` Syscalls | `nasm` (assemble the examples) |
| `0x0A`–`0x0D` Exploitation | `gdb`+`pwndbg`, `pwntools`, [ROPgadget](https://github.com/JonathanSalwan/ROPgadget) |
| `0x0E` Network | `wireshark`, `tcpdump`, `nmap`, `netcat` |
| `0x0F` Decompilers | [Ghidra](https://ghidra-sre.org/) (needs a JDK) |
| `0x10` Crypto | `john`, `hashcat`, [CyberChef](https://gchq.github.io/CyberChef/) (browser) |
| `0xFD` Forensic | [Volatility 3](https://github.com/volatilityfoundation/volatility3), `binwalk`, `sleuthkit`, `exiftool` |
| `0x12` Web | [Burp Suite](https://portswigger.net/burp) or OWASP ZAP, `curl`, [ffuf](https://github.com/ffuf/ffuf) |
| `0xFF` Android | `adb`/`fastboot`, `frida`/`objection` (see [`setup_android-ssl-pining.sh`](0xFF%20-%20android/setup_android-ssl-pining.sh)) |

## Disabling protections for exploitation practice

Several chapters assume the relaxed conditions used by tutorials. To re-create
them (inside your throwaway VM only):

```sh
# Turn off ASLR system-wide (revert with value 2). Needs root. See 0x0A.
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space

# Compile a target with protections off. See 0x02.
gcc -g -O0 -fno-stack-protector -z execstack -no-pie vuln.c -o vuln
```

Re-enable ASLR (`echo 2 | sudo tee /proc/sys/kernel/randomize_va_space`) or just
reboot/restore your snapshot when done.

## Practice targets

The exploitation chapters use **Phoenix** from
[exploit.education](https://exploit.education/phoenix/getting-started/) — a small
VM image full of deliberately vulnerable binaries. Download it and run it in your
hypervisor. For the other categories, see the "Where to practice" list in the
[main README](README.md).
