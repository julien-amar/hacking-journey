#include <stdint.h>

int main()
{
    volatile int a = 0;
    volatile int b = 1;

    if (a > 0xff)
    {
        b++;
    }

    while (a < 10)
    {
        a++;
    }

    for (a = 0; a < 10; a++)
    {
        b++;
    }

    return 0;
}
