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

To execute the script you are currently writing, you can type in VIM:
`:!python %`

```sh
$> python print.py argument "with quotes"    and    spaces
['hello.py', 'argument', 'with quotes', 'and', 'spaces']
```

## VIM configuration

You can configure indentation using the following VIM command:
```
:set expandtab shiftwidth=4 softtabstop=4
```