/*
 * Part A, required: which machine am I running on?
 *
 * Prints what this binary was built for, and what the machine it is running
 * on says about itself. Run the one for your laptop, then the armhf one.
 */
#include <limits.h>
#include <stdio.h>
#include <sys/utsname.h>

#if defined(__x86_64__)
#define BUILT_FOR "x86_64 (64-bit x86)"
#elif defined(__aarch64__)
#define BUILT_FOR "aarch64 (64-bit ARM)"
#elif defined(__arm__)
#define BUILT_FOR "armhf (32-bit ARM)"
#else
#define BUILT_FOR "something else"
#endif

int main(void)
{
    struct utsname u;
    uname(&u);

    printf("built for       : %s\n", BUILT_FOR);
    printf("machine reports : %s\n", u.machine);
    printf("pointer size    : %zu bits\n", sizeof(void *) * 8);
    printf("long size       : %zu bits\n", sizeof(long) * 8);
    printf("plain char is   : %s\n", CHAR_MIN < 0 ? "signed" : "unsigned");
    return 0;
}
