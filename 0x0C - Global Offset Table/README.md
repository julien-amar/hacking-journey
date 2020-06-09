# Global Offset Table (GOT)

In order to reduce binary size, from reusable protion of code can be extrated and shared: a library (*.so, *.dll) could be provided.
All required libraries are loaded on program start. However, the program have to know on which address those functions are available (especially when ASLR is enabled).
The GOT's purpose is to answer this problematic, it basically keep a reference to dependent library functions.

By default, the GOT is writable, as the resolution of such function is done by the loader (ld.so) during the first call.
Some protection exist to make the GOT readonly.

Documentation: https://ctf101.org/binary-exploitation/what-is-the-got/
Documentation: https://ctf101.org/binary-exploitation/relocation-read-only/

# Procedure Linkage Table (PLT)

To do the resolution, some trampoline functions (PLT) are injected in the program, to interface with the GOT, those functions are responsible for calling the dynamic linker.

# Dynamic loading

Along your program, it is possible to load additionnal information (or even override some library or methods), by configuring ld, through:
* Environment variable LD_LIBRARY_PATH or LD_PRELOAD. Except if the executable is a setuid/setgid binary, in which case it is ignored.
* From the cache file /etc/ld.so.cache which contains a compiled list of candidate libraries previously found in the augmented library path.
* In the default path /lib, and then /usr/lib.

For mroe details: https://manpages.ubuntu.com/manpages/precise/man8/ld.so.8.html
