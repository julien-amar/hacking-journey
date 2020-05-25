# Introduction to Python

## Hello world in Python

```py
import sys

if len(sys.argv)==2:
    print("Knock, knock, {0}".format(sys.argv[1]))
else:
    sys.stderr.write("Usage: {0} <name>".format(sys.argv[0]))
```

## Execution

To execute the script you are currently writing, you can type in `VIM`:
`:!python %`

You can also execute it in a shell:
```sh
$> python print.py argument "with quotes"    and    spaces
['hello.py', 'argument', 'with quotes', 'and', 'spaces']
```

You can also execute a single line of python to exploit a specific binary:
```sh
$> python -c 'print "A"*(4+16*3+14)' | ./exploit
Congratulation, you changed the variable, using a buffer overflow technique.
```

## Conversions

| From  | To    | Code                     | Result      |
|-------|-------|--------------------------|-------------|
| dec   | bin   | bin(123)                 | '0b1111011' |
| dec   | hex   | hex(123)                 | '0x7b'      |
| bin   | dec   | int('0x1111011', 2)      | 123         |
| bin   | hex   | hex(int('0x1111011', 2)) | '0x7b'      |
| hex   | dec   | int('0x7b', 16)          | 123         |
| hex   | bin   | bin(int('0x7b', 16))     | '0b1111011' |
| hex   | ascii | "\x41\x42"               | 'AB'        |
| ascii | hex   | b"AB".hex()              | '4142'      |

## Packing / Unpacking

```python
import struct
value = struct.unpack("I", b"ABCD")[0]
value # 1145258561
hex(value) # '0x44434241'
```

As we can notice in the example above, the hex conversion is **reversed**. This is due to **endianess** of the system.
In little endian architecture, the least significant bit is stored on the lowest address.

The packing library handle the endianess:

```python
value = struct.unpack(">I", b"ABCD")[0]
value # 1094861636
hex(value) # '0x41424344'
```

_Documentation: https://docs.python.org/3/library/struct.html & https://en.wikipedia.org/wiki/Endianness_

## VIM configuration

Indentation is import when working in python, you can configure it using the following VIM command:
```
:set expandtab shiftwidth=4 softtabstop=4
```