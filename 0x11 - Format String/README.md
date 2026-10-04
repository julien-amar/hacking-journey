# Format string vulnerabilities

A format-string bug is one of the classic memory-corruption bugs, sitting
neatly between the stack (`0x0A`) and heap (`0x0D`) chapters. It is powerful
because a single mistake gives you both an **arbitrary read** and an
**arbitrary write**.

## The bug

Functions in the `printf` family take a *format string* that describes how to
interpret the remaining arguments:

```c
printf("%s is %d years old\n", name, age);
```

The bug appears when **user input is passed as the format string itself**:

```c
printf(user_input);            // VULNERABLE
// should be:
printf("%s", user_input);      // safe
```

Now the attacker controls the format directives. The problem: `printf` doesn't
know how many arguments were *actually* passed. It trusts the format string and
reads arguments from the usual argument registers/stack positions regardless —
so extra `%x`/`%p` directives make it read values that were never meant to be
arguments.

## What it gives you

### Leak memory (arbitrary read)

Each conversion specifier makes `printf` fetch and print the "next argument":

```
%p %p %p %p %p %p        # dump successive words from the stack / arg registers
%7$p                     # directly fetch the 7th argument ("direct parameter access")
%s                       # treat the next argument as a pointer and print the string there
```

Because your input string is *itself on the stack*, you can place a target
address in it and then use `%s` to dereference and print whatever lives at that
address — e.g. leak a GOT entry to defeat ASLR (chapter `0x0A`), or read a
secret. `%7$p`-style indexing lets you point straight at the word you want
without dumping everything before it.

### Write memory (arbitrary write)

The lesser-known specifier **`%n`** writes *the number of bytes printed so far*
into the integer pointed to by its argument:

```c
int written;
printf("hello%n", &written);   // written becomes 5
```

Combined with controlling that argument (an address placed in your input) and
controlling the printed count (via field-width padding like `%100x`), you can
write an arbitrary value to an arbitrary address — typically a **GOT entry**
(chapter `0x0C`) or a return address, redirecting execution. Width modifiers
`%hn` (2 bytes) and `%hhn` (1 byte) let you write a large value in small,
controllable steps.

## Finding it

The smell test: give the program a format string as input and see if it
interprets it.

```sh
$ ./vuln "AAAA %p %p %p %p"
AAAA 0x7fffffffe4a0 0x1 0x41414141 0x2520702500000000
#                            ^ our "AAAA" showed up -> format string bug,
#                              and it's the 3rd argument from here
```

Seeing your own input (`0x41414141` = `AAAA`) echoed back as a leaked value
confirms the bug *and* tells you the parameter index to use for reads/writes.

> **pwntools** automates the hard parts: `fmtstr_payload(offset, {addr: value})`
> builds the exact `%n`-based payload for a given write, which is otherwise
> fiddly to compute by hand.

## Mitigations

* **Never pass user input as a format string.** `printf(user)` → `printf("%s", user)`.
* Compilers warn about this: build with `-Wformat -Wformat-security` (and treat
  it as an error).
* `FORTIFY_SOURCE` (`-D_FORTIFY_SOURCE=2`) rejects `%n` writes to non-constant
  format strings at runtime in many cases.
* Full RELRO (chapter `0x0C`) removes the GOT as a convenient write target.

## Practice

* exploit.education Phoenix *format-*: https://exploit.education/phoenix/
* pwn.college "Program Interaction / format string" modules
