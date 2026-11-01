/*
 * Part B, optional: one million tiny writes to /dev/null.
 *
 * Each write asks the kernel to do the work. Under QEMU, whose kernel is
 * that? Compare the slowdown with speed.
 */
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define WRITES 1000000

static double now_ms(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000.0 + t.tv_nsec / 1e6;
}

int main(void)
{
    int fd = open("/dev/null", O_WRONLY);
    if (fd < 0) {
        perror("open /dev/null");
        return 1;
    }

    double t0 = now_ms();
    for (int i = 0; i < WRITES; i++)
        if (write(fd, "x", 1) != 1)
            return 1;
    double t1 = now_ms();

    close(fd);
    printf("syscalls: %d writes of 1 byte, %6.0f ms\n", WRITES, t1 - t0);
    return 0;
}
