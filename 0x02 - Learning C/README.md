# Introduction to C

## Hello world in C

```c
#include <stdio.h>

int main(int argc, char*argv[]) {
    if (argc == 2)
    {
        printf("Knock, knock, %s\n", argv[1]);
    }
    else
    {
        fprintf(stderr, "Usage: %s <name>\n", argv[0]);
        return 1;
    }
    return 0;
}
```

`main` is the entry point of our program
`argc` contains the number of arguments passed to the executable
`argv` contains the values of arguments passed to the executable

All C functions are documented in `man`.
By example, for `printf`,  you can access it through `man 3 printf`.

## Compilation

You can now compile your program with the following:
```sh
$> gcc hello.c -o hello -Wall

$> ./hello
Usage: ./hello <name>

$> ./hello Neo
Knock, knock, Neo

# The program expect only 1 additionnal parameter
$> ./hello Neo the hero
Usage: ./hello <name>

# Pass a string (with multiple words) as input parameter
$> ./hello "Neo the hero"
Knock, knock, Neo the hero

# Pass the user environment variable's value as a parameter
$> ./hello $USER
Knock, knock, ange

# Pass "$USER" as a parameter, without being interpreted by the shell. 
$> ./hello \$USER
Knock, knock, $USER

$> ./hello '$USER'
Knock, knock, $USER
```

## Return value

Return value is accessible through `$?` variable just after executing your program.