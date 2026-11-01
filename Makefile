# Builds every program for all three targets: bin/<program>-<target>
#
#   x86_64   most laptops
#   aarch64  64-bit ARM: the board, Apple Silicon
#   armhf    32-bit ARM: no laptop runs it natively
#
# Binaries are linked statically: each one carries its own C library, so it
# runs on any Linux with the right CPU, the board included, with nothing
# installed. whereami-dynamic-armhf is the one exception, on purpose.

TARGETS    := x86_64 aarch64 armhf
CC_x86_64  := x86_64-linux-gnu-gcc
CC_aarch64 := aarch64-linux-gnu-gcc
CC_armhf   := arm-linux-gnueabihf-gcc
CFLAGS     := -O2 -Wall
LDLIBS     :=

vpath %.c part-a part-b
PROGRAMS := $(basename $(notdir $(wildcard part-a/*.c part-b/*.c)))
BINS     := $(foreach p,$(PROGRAMS),$(foreach t,$(TARGETS),bin/$(p)-$(t))) \
            bin/whereami-dynamic-armhf

all: $(BINS)

bin:
	mkdir -p bin

bin/whereami-dynamic-armhf: whereami.c | bin
	$(CC_armhf) $(CFLAGS) -o $@ $< $(LDLIBS)

bin/%-x86_64: %.c | bin
	$(CC_x86_64) $(CFLAGS) -static -o $@ $< $(LDLIBS)

bin/%-aarch64: %.c | bin
	$(CC_aarch64) $(CFLAGS) -static -o $@ $< $(LDLIBS)

bin/%-armhf: %.c | bin
	$(CC_armhf) $(CFLAGS) -static -o $@ $< $(LDLIBS)

bench: all
	tools/bench.sh

clean:
	rm -rf bin

.PHONY: all bench clean
