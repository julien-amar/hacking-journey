# Introduction to Python

Python is the go-to language for scripting exploits and CTF tooling: it is quick
to write, has libraries for almost everything, and makes it easy to build and
send raw bytes to a program.

> **Python 2 vs Python 3.** Many older exploitation write-ups (including some of
> the examples in this repository) were written for Python 2, where `print` is a
> statement: `print "A"*64`. Python 2 is end-of-life — use Python 3, where
> `print` is a function: `print("A"*64)`. The other big difference that matters
> for exploitation is that Python 3 separates text (`str`) from raw bytes
> (`bytes`); when sending a payload you almost always want `bytes`, e.g.
> `b"A"*64`.

## Hello world in Python

```py
import sys

if len(sys.argv) == 2:
    print("Knock, knock, {0}".format(sys.argv[1]))
else:
    sys.stderr.write("Usage: {0} <name>\n".format(sys.argv[0]))
```

## Execution

To run the Python script you are currently editing from inside Vim:
`:!python3 %` (`%` expands to the current file name).

You can also run it from a shell. Note how the shell splits arguments on spaces
unless they are quoted:

```sh
$ python3 print.py argument "with quotes"    and    spaces
['print.py', 'argument', 'with quotes', 'and', 'spaces']
```

You can also run a single line of Python — handy for generating an exploit
payload and piping it into a vulnerable binary:

```sh
# Python 3 (note: print to stdout as bytes using sys.stdout.buffer)
$ python3 -c 'import sys; sys.stdout.buffer.write(b"A"*(4+16*3+14))' | ./exploit
Congratulations, you changed the variable using a buffer-overflow technique.
```

## Conversions

| From  | To    | Python 3 code                  | Result        |
|-------|-------|--------------------------------|---------------|
| dec   | bin   | `bin(123)`                     | `'0b1111011'` |
| dec   | hex   | `hex(123)`                     | `'0x7b'`      |
| bin   | dec   | `int('0b1111011', 2)`          | `123`         |
| bin   | hex   | `hex(int('0b1111011', 2))`     | `'0x7b'`      |
| hex   | dec   | `int('0x7b', 16)`              | `123`         |
| hex   | bin   | `bin(int('0x7b', 16))`         | `'0b1111011'` |
| hex   | ascii | `bytes.fromhex('4142')`        | `b'AB'`       |
| ascii | hex   | `b"AB".hex()`                  | `'4142'`      |

> In Python 3, `int('0b1111011', 2)` works with or without the `0b` prefix as
> long as you pass base `2`. The original notes used `int('0x1111011', 2)`,
> which mixes a hex-looking prefix with base 2 — avoid that; be explicit about
> which base a string is in.

## Packing / unpacking

Exploits constantly need to convert an integer (like a memory address) to the
exact byte sequence the target expects. The `struct` module does this:

```python
import struct

value = struct.unpack("I", b"ABCD")[0]

value       # 1145258561
hex(value)  # '0x44434241'
```

Notice the bytes come out "reversed" (`ABCD` → `0x44434241`). That is because of
the system's **endianness**. On a little-endian architecture (x86/x64), the
least-significant byte is stored at the lowest address.

The format string controls endianness explicitly:

```python
value = struct.unpack(">I", b"ABCD")[0]   # ">" = big-endian

value       # 1094861636
hex(value)  # '0x41424344'
```

Common format characters: `<`/`>` select little/big-endian; `I` = 4-byte
unsigned int, `Q` = 8-byte unsigned (use `<Q` to pack a 64-bit address on
x86-64).

*Docs: <https://docs.python.org/3/library/struct.html> &
<https://en.wikipedia.org/wiki/Endianness>*

## pwntools — the exploitation toolkit

For anything beyond a trivial payload, use [pwntools](https://docs.pwntools.com/).
It wraps packing, process/socket I/O, ELF parsing, shellcode and ROP into one
library and is the de-facto standard in CTFs:

```python
from pwn import *

context.arch = 'amd64'          # sets endianness, pointer size, shellcode arch

payload  = b"A" * 64            # fill the buffer
payload += p64(0x400abd)        # p64() packs a 64-bit little-endian address
                                # (p32() for 32-bit; u64()/u32() to unpack)

io = process('./vuln')          # or: remote('host', 1337)
io.sendline(payload)
io.interactive()                # drop into an interactive session (e.g. a shell)
```

Install with `pip install pwntools`.

## Interactive session

To get an interactive Python shell when your script exits (useful to inspect
state after an exploit runs), either:

* set the `PYTHONINSPECT` environment variable before running the script, or
* run it with the `-i` flag: `python3 -i exploit.py`.

## Built-ins

Python's built-in functions are available through this dictionary:
`__builtins__.__dict__['print']('Hello!')`. By overriding entries in it you can
change the default implementation of a built-in — a trick that occasionally
shows up in Python sandbox-escape challenges.

## Vim configuration

Indentation is significant in Python. Configure Vim to use 4 spaces instead of
tabs:

```
:set expandtab shiftwidth=4 softtabstop=4
```
