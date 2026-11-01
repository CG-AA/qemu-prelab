/*
 * Part B, optional: count primes by trial division.
 *
 * Compare armhf with aarch64 as well as with your laptop. Both ARM builds run
 * under the same emulator on an x86 laptop. Is the emulator the only reason
 * for the difference? (In the toolbox, arm-linux-gnueabihf-objdump -d shows the code.)
 */
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define LIMIT 2000000u

static double now_ms(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000.0 + t.tv_nsec / 1e6;
}

int main(void)
{
    double t0 = now_ms();
    uint32_t count = 0;
    for (uint32_t n = 2; n < LIMIT; n++) {
        int prime = 1;
        for (uint32_t d = 2; d * d <= n; d++) {
            if (n % d == 0) {
                prime = 0;
                break;
            }
        }
        count += prime;
    }
    double t1 = now_ms();

    printf("primes  : %u below %u, %6.0f ms\n", count, LIMIT, t1 - t0);
    return 0;
}
