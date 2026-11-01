/*
 * Part B, optional: copy a 64 MB buffer 40 times.
 *
 * Moving memory, like copying camera frames. How does its slowdown compare
 * with speed and with floats?
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE   (64u * 1024 * 1024)
#define COPIES 40

static double now_ms(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000.0 + t.tv_nsec / 1e6;
}

int main(void)
{
    char *src = malloc(SIZE), *dst = malloc(SIZE);
    if (!src || !dst) {
        fprintf(stderr, "not enough memory\n");
        return 1;
    }
    memset(src, 1, SIZE);
    memset(dst, 2, SIZE);               /* touch every page before timing */

    double t0 = now_ms();
    for (int i = 0; i < COPIES; i++) {
        src[i] = (char)i;               /* keep the copies from being skipped */
        memcpy(dst, src, SIZE);
    }
    double t1 = now_ms();

    double gb = (double)SIZE * COPIES / 1e9;
    printf("memory  : %.1f GB copied, %.1f GB/s, %6.0f ms (check %d)\n",
           gb, gb / ((t1 - t0) / 1000), t1 - t0, dst[COPIES - 1]);
    free(src);
    free(dst);
    return 0;
}
