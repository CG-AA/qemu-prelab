/*
 * Part B, optional: multiply two float matrices.
 *
 * This is the kind of work NumPy and OpenCV do all day. Compare the slowdown
 * with the float line of speed. Also compare the checksums: are they
 * the same on every target?
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 600

static float a[N][N], b[N][N], c[N][N];

static double now_ms(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000.0 + t.tv_nsec / 1e6;
}

int main(void)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            a[i][j] = (float)((i * 7 + j * 3) % 17) / 17.0f;
            b[i][j] = (float)((i * 5 + j * 11) % 13) / 13.0f;
        }

    double t0 = now_ms();
    for (int i = 0; i < N; i++)
        for (int k = 0; k < N; k++) {
            float aik = a[i][k];
            for (int j = 0; j < N; j++)
                c[i][j] += aik * b[k][j];
        }
    double t1 = now_ms();

    double sum = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            sum += c[i][j];

    printf("floats  : %dx%d matrix multiply, checksum %.6f, %6.0f ms\n", N, N, sum, t1 - t0);
    return 0;
}
