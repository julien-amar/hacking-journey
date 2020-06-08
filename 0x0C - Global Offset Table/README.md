# Global Offset Table (GOT)

In order to reduce binary size, from reusable protion of code can be extrated and shared: a library (*.so, *.dll) could be provided.
All required libraries are loaded on porogram start. However, the program have to know on which address those functions are available (especially when ASLR is enabled).
The GOT's purpose is to answer this problematic, it basically keep a reference to dependent library functions.

By default, the GOT is writable, as the resolution of such function is done by the loader (ld.so) during the first call.
Some protection exist to make the GOT readonly.

Documentation: https://ctf101.org/binary-exploitation/what-is-the-got/
Documentation: https://ctf101.org/binary-exploitation/relocation-read-only/

# Procedure Linkage Table (PLT)

To do the resolution, some trampoline functions (PLT) are injected in the program, to interface with the GOT, those functions are responsible for calling the dynamic linker.
