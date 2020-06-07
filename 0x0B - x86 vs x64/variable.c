#include <stdint.h>

struct r {
    uint64_t r1;
    uint32_t r2;
};

int main() {
    // different datatypes in C
    volatile int a = 0x1234;
    volatile unsigned int b = 0x1234;
    volatile uint32_t c = 0x1234;
    volatile uint64_t d = 0x1234;
    volatile int e = -0x1234;
    volatile unsigned int f = -0x1234;
    volatile float g = 0;
    volatile float h = 12.34;
    volatile float i = -12.34;
    volatile double j = 0;
    volatile double k = 12.34;
    volatile double l = -12.34;
    volatile uint32_t m[10] = {0x0, 0x1, 0x22, 0x333, 0x4444};
    volatile uint32_t m2 = m[2];
    volatile char n = 'A';
    // cast a character into an integer
    volatile uint8_t o = 'B';
    volatile const char *p = "AAAA";
    volatile char *q = "BBBB";

    // Struct
    volatile struct r s = {0};
    s.r1 = 0x41414141414141;
    s.r2 = 0x414141;

    // f is 64bit. So what happens on 32bit?
    f += 0x4141414141;

    int t = a++;
    int u = ++a;
    return 0;
}