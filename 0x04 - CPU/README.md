# CPU

## Registers

Depending on the architecture, your CPU have a certains number of registers. You can consider registers as global variables.

The bitness of your machine (32 or 64 bits), establish the width of the registers.

Some registers are just subpart of another register's value:
```
|__64__|__56__|__48__|__40__|__32__|__24__|__16__|__8___|
|__________________________RAX__________________________|
|xxxxxxxxxxxxxxxxxxxxxxxxxxx|____________EAX____________|
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx|_____AX______|
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx|__AH__|__AL__|
```

Amoung those registers, some are specials:
* the *Program Counter* / *Instruction pointer* (RIP/EIP/IP/PC depending on the architecture), indicate which instruction (by referencing its address) we are executing next
* the zero flag is a register that is updated after an operation like an addition or substraction based on the result of the operation. It's value is set depending on the result of the operation (> 0).

## Execution flow

The control flow of the program is done through GO TO instructions:
* jumps: `jmp`, `jne`, `je`
* branch: `bne`, `be`
* call: `call`

### Jump

Jump instruction is similar to changing the `rip` register to the specified value.

### Call

A function can be called in multiple places, to allow this behaviour, we will need to know where to go back when the function returned its result. To do so, the address where the call (`rip` register) was made is pushed to the stack.

The `ret` instruction, will store the function result in `eax`, and `pop` the address to continue where we originaly came from by restoring the previous stack frame information.

## Memory management

The number of register is limited, therefor, everything that does not fit in the registers should be stored in memory (HEAP or STACK).

```
+-------------+ <- 0xffffffff
|             |
|    STACK    |
|             |
+vvvvvvvvvvvvv+ 
|             |
|             |
+^^^^^^^^^^^^^+ 
|             |
|    HEAP     |
|             |
+-------------+ 
|             |
|  PROGRAM    |
|             |
+-------------+ <- 0x00000000
```

## Heap

The Heap is a block of memory that aims at storing long term information through the execution of your program.

You can interact with it through the instructions `mov`:

```c
// Store the referenced value (address: 0x14) in the register eax
mov eax, [0x14]
```

## Stack

The stack is a memory space where you can store some information through the execution of your program.

The top of the stack is referenced by the register `sp`, `esp` or `rsp`.

You can interact with it through the instructions `pop` and `push`:

```c
// Push 5 to the stack, and decrease the value of esp.
push 0x05
// Pop 5 from the stack, and increase the value of esp.
pop eax
```

Stack is composed of multiple frames, they compose the call stack. Each stack frame reference the previous one:
```
+--------------+
|   PREVIOUS   |
| STACK  FRAME |
+--------------+ 
| PREVIOUS RBP |
+--------------+ <- current RBP (begining of current stack frame)
|              |
|     DATA     |
|              |
+--------------+ <- current RSP (end of current stack frame)
```

## Resources

You can pratice with the following website:
* https://microcorruption.com/ (account: matasano)
* https://www.recurse.com/blog/7-understanding-c-by-learning-assembly