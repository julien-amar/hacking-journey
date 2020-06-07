#include <stdint.h>

// function without parameter and return value
void first_function()
{
    __asm__("nop");
}

// function with integer return value
int second_function()
{
    return 0x1234;
}

// function with parameter and return value
int third_function(int x)
{
    return x + 1;
}

// function with multiple parameters and return value
int fourth_function(int x, int y)
{
    return x + y;
}

// function with a lot of parameters
int fifth_function(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p)
{
    return a + b + c + d + e + f + g + h + i + j + k + l + m + n + o + p;
}

int main()
{
    first_function();
    volatile int a = second_function();
    volatile int b = third_function(a);
    volatile int c = fourth_function(a, b);
    volatile int d = fifth_function(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    return 0;
}
