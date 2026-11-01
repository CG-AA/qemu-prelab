/*
 * Part B, required: what does QEMU slow down?
 *
 * The same sensor filter twice, then a wait, each timed separately:
 *   integer  filter 200 million fake readings with integer math
 *   float    filter 100 million fake readings with float math
 *   wait     sleep 10 x 100 ms, like waiting for a slow sensor
 *
 * Run it for your laptop and as armhf. Write down all three lines for both.
 */
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define INT_READINGS   200000000u
#define FLOAT_READINGS 100000000u

static double now_ms(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000.0 + t.tv_nsec / 1e6;
}

/* Fake readings: a noisy level that steps between 500 and 700 every ~4 million
 * readings. xorshift32 makes the noise, the same on every machine. */
static uint32_t next_noise(uint32_t x)
{
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

/* Smooth the readings and count how often the smoothed value rises above
 * 600. Integer version: the average is kept scaled by 16. */
static uint32_t alarms_integer(uint32_t n)
{
    uint32_t noise = 2463534242u, alarms = 0;
    int32_t smooth = 500 * 16;
    int above = 0;

    for (uint32_t i = 0; i < n; i++) {
        noise = next_noise(noise);
        int32_t level = ((i >> 22) & 1) ? 700 : 500;
        int32_t reading = level + (int32_t)(noise & 0xff) - 128;
        smooth += reading - (smooth >> 4);
        int now_above = (smooth >> 4) > 600;
        alarms += now_above && !above;
        above = now_above;
    }
    return alarms;
}

/* The same filter, written the way most people would: with floats.
 * 0.0625 is a power of two, so the multiply is exact and every CPU counts
 * the same alarms. Other constants could round differently on aarch64,
 * which fuses the multiply and add into one instruction. */
static uint32_t alarms_float(uint32_t n)
{
    uint32_t noise = 2463534242u, alarms = 0;
    float smooth = 500.0f;
    int above = 0;

    for (uint32_t i = 0; i < n; i++) {
        noise = next_noise(noise);
        float level = ((i >> 22) & 1) ? 700.0f : 500.0f;
        float reading = level + (float)(noise & 0xff) - 128.0f;
        smooth += 0.0625f * (reading - smooth);
        int now_above = smooth > 600.0f;
        alarms += now_above && !above;
        above = now_above;
    }
    return alarms;
}

int main(void)
{
    double t0 = now_ms();
    uint32_t int_alarms = alarms_integer(INT_READINGS);
    double t1 = now_ms();
    uint32_t float_alarms = alarms_float(FLOAT_READINGS);
    double t2 = now_ms();
    struct timespec tenth = {0, 100000000};
    for (int i = 0; i < 10; i++)
        nanosleep(&tenth, NULL);
    double t3 = now_ms();

    printf("integer : %u readings, %u alarms, %6.0f ms\n", INT_READINGS, int_alarms, t1 - t0);
    printf("float   : %u readings, %u alarms, %6.0f ms\n", FLOAT_READINGS, float_alarms, t2 - t1);
    printf("wait    : 10 x 100 ms of sleep,          %6.0f ms\n", t3 - t2);
    return 0;
}
