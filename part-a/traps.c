/*
 * Part A, optional: same source, different answers.
 *
 * Run it for all three targets and put the outputs side by side.
 * Which lines change? Which one did you expect to change but didn't?
 */
#include <stdio.h>
#include <string.h>

struct reading {            /* imagine this is written to a file or sent over UDP */
    char  sensor;
    long  timestamp_ms;
    void *next;
};

int main(void)
{
    char c = (char)200;
    printf("1. (char)200 is %d\n", c);

    volatile long days = 30;
    long day_ms = 24L * 60 * 60 * 1000;
    printf("2. 30 days in ms is %ld\n", day_ms * days);

    printf("3. struct reading is %zu bytes\n", sizeof(struct reading));

    unsigned char bytes[4] = {1, 0, 0, 0};
    unsigned int n;
    memcpy(&n, bytes, sizeof n);
    printf("4. the bytes 01 00 00 00 read as an int: %u\n", n);
    return 0;
}
