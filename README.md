# QEMU prelab: quick start

Run a program built for another CPU, then time it. About 20 minutes.
For each step, watch the demo first, then do it yourself.

You need **Docker** (on Windows and macOS, Docker Desktop must be running) and **git**.
On Windows, use **PowerShell**, not Command Prompt or Git Bash.

Each step says where to type: **on your laptop** (a normal terminal) or **in the
container** (after step 1, until you type `exit`). In the container, your prompt
starts with `root@`.

## Before class: get the kit

On your laptop:

```sh
git clone https://github.com/CG-AA/qemu-prelab.git qemu-prelab
cd qemu-prelab
docker pull debian:bookworm-slim
```

## Step 1: start a Linux container

On your laptop, go to the kit folder and start the container:

```sh
cd ~/qemu-prelab
docker run --rm -it -v "${PWD}:/work" -w /work debian:bookworm-slim
```

(Cloned it somewhere else? `cd` there instead.) Your prompt changes to
`root@…:/work#`. You are now in the container. Then:

```sh
ls bin
```

`bin/` holds every program built for three CPUs:

| Suffix     | CPU        | Runs natively on                                  |
|------------|------------|---------------------------------------------------|
| `-x86_64`  | 64-bit x86 | most laptops                                      |
| `-aarch64` | 64-bit ARM | the board, ARM laptops such as Apple Silicon Macs |
| `-armhf`   | 32-bit ARM | no laptop                                         |

## Part A: run the wrong binary

### Step 2: which machine are you on?

In the container (your prompt shows `root@…:/work#`):

```sh
uname -m
./bin/whereami-$(uname -m)
```

### Step 3: run a binary for another CPU

Write down first: will it run? Then, in the container:

```sh
./bin/whereami-armhf
```

- **It prints `machine reports : armv7l`**: something ran it for you.
- **`cannot execute binary file: Exec format error`**: nothing on your laptop can run
  ARM code yet. On Linux, do step 3b. On Windows or macOS, check your prompt: if it
  doesn't start with `root@`, you typed it on your laptop, so repeat step 1.

Write down: who ran it? How could you tell? Then go to step 4.

### Step 3b (Linux only): install QEMU

Only if step 3 printed `Exec format error`. On your laptop, in a second terminal
(leave the container open):

```sh
sudo apt update
sudo apt install qemu-user-static
```

If apt says `qemu-user-static` has no installation candidate (Ubuntu 26.04 and
newer), run `sudo apt install qemu-user-binfmt` instead.

Then in the container, run `./bin/whereami-armhf` again. It now prints
`machine reports : armv7l`. Not on Ubuntu? See "If something goes wrong" below.

## Part B: what does QEMU slow down?

### Step 4: time it on your own CPU

Write down first: how many times slower will the armhf build be? Then, in the container:

```sh
./bin/speed-$(uname -m)
```

It runs the same sensor filter twice, with integer math and with float math, then
sleeps for one second. Each line is timed separately.

### Step 5: time it on the emulated CPU

In the container:

```sh
./bin/speed-armhf
```

### Step 6: write it down

| Build            | integer (ms) | float (ms) | wait (ms) |
|------------------|--------------|------------|-----------|
| my laptop        |              |            |           |
| armhf (emulated) |              |            |           |

1. Which line slowed down the most, which the least, and which not at all?
2. Did the number of alarms change?
3. A program takes 2 seconds in an armhf container. How long will it take on the
   board? Can this table tell you?

## Optional: more programs

In the container:

| Run                                                                  | Question                                                          |
|----------------------------------------------------------------------|-------------------------------------------------------------------|
| `./bin/traps-x86_64`, `./bin/traps-aarch64`, `./bin/traps-armhf`     | Same source, different answers. Which lines change? Which did you expect to change, but didn't? |
| `./bin/primes-aarch64`, `./bin/primes-armhf`                         | On an x86 laptop both are emulated (on an Apple Silicon Mac, aarch64 runs natively). Is QEMU the only difference? |
| `./bin/floats-armhf`, `./bin/syscalls-armhf`, `./bin/memory-armhf`   | Compare each with the build for your laptop. Which slow down like the float line, and which barely slow down? |
| `./bin/whereami-dynamic-armhf`                                       | The error says a file is missing. Which file?                     |
| `bash tools/bench.sh`                                                | Every Part B program on all three CPUs at once. Takes under a minute. |

## Optional, best at home: from your laptop's terminal

These download three more images (about 150 MB), so do them at home rather than on
the classroom network. Type `exit` to leave the container first. Then, on your laptop:

```sh
docker run --rm -v "${PWD}:/work" python:3.12-slim python /work/part-b/speed.py
docker run --rm -v "${PWD}:/work" --platform linux/arm/v7 arm32v7/python:3.12-slim python /work/part-b/speed.py
docker run --rm -v "${PWD}:/work" --platform linux/arm/v7 arm32v7/debian:bookworm-slim /work/bin/whereami-dynamic-armhf
```

- The first two run the integer filter in Python. Is Python hurt more or less than C?
- The last one runs `whereami-dynamic-armhf` in a real 32-bit ARM container. Why does
  it work here, when it failed in the Debian container?

## Before the board day

If you are still in the container, type `exit`. Keep `bin/speed-aarch64`. It carries
its own C library, so it runs on the board as it is. Write down what you think the
board's three lines will be, and check on the first day with the board.

## If something goes wrong

- **`failed to connect to the docker API`, `Cannot connect to the Docker daemon` or
  `error during connect`**: start Docker Desktop and wait until it says it is running.
- **`ls: cannot access 'bin'`**: you started the container outside the kit folder.
  Type `exit`, then repeat step 1.
- **`No such file or directory` for a program**: check the name with `ls bin` (the Tab
  key completes names). If `ls bin` fails too, see the line above.
- **`uname` or `./bin/...` not recognized, or `zsh: exec format error`**: you typed it
  on your laptop, not in the container. Repeat step 1.
- **Windows: errors about `${PWD}`, `invalid reference format`, or a path like
  `C:/Program Files/Git/work`**: you are in Command Prompt or Git Bash. Open
  PowerShell and repeat step 1.
- **`Permission denied` when running a program**: in the container, run `chmod +x bin/*`.
- **Linux: `permission denied` on `docker.sock`**: run `sudo usermod -aG docker $USER`,
  then log out and back in.
- **`Exec format error` without Docker Desktop and not on Ubuntu** (another Linux,
  Docker Engine in WSL, Colima): register QEMU with
  `docker run --privileged --rm tonistiigi/binfmt --install arm`. This needs regular
  (not rootless) Docker and lasts until you reboot.

## Build them yourself (optional)

The programs in `bin/` are built from `part-a/` and `part-b/` with the toolbox in this
folder. It downloads about 200 MB and takes about 900 MB of disk.

```sh
docker build -t prelab-tools .
docker run --rm -v "${PWD}:/work" prelab-tools make -B
```
