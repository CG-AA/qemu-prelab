"""
Part B, optional: a shorter version of speed.c, in Python.

Your .py file runs anywhere. The interpreter underneath it is a binary,
built for one CPU. Run this in a Python container for your laptop and in an
armhf one (commands in README.md). How does the slowdown compare with the C
version?
"""
import time

READINGS = 2_000_000
MASK = 0xFFFFFFFF


def count_alarms(n):
    noise = 2463534242
    smooth = 500 * 16
    above = False
    alarms = 0
    for i in range(n):
        noise ^= (noise << 13) & MASK
        noise ^= noise >> 17
        noise ^= (noise << 5) & MASK
        level = 700 if (i >> 18) & 1 else 500
        reading = level + (noise & 0xFF) - 128
        smooth += reading - (smooth >> 4)
        now_above = (smooth >> 4) > 600
        if now_above and not above:
            alarms += 1
        above = now_above
    return alarms


t0 = time.perf_counter()
alarms = count_alarms(READINGS)
t1 = time.perf_counter()
time.sleep(1.0)
t2 = time.perf_counter()

print(f"{f'compute : {READINGS} readings, {alarms} alarms,':<40}{(t1 - t0) * 1000:6.0f} ms")
print(f"{'wait    : 1 s of sleep,':<40}{(t2 - t1) * 1000:6.0f} ms")
