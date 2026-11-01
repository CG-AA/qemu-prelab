#!/usr/bin/env bash
# Optional: run every Part B program for all three targets and print how many
# times slower each one is than the build for this machine. Run `make` first.
set -u
cd "$(dirname "$0")/.."

native=$(uname -m)
targets="x86_64 aarch64 armhf"
TIMEFORMAT=%R

seconds() {   # wall-clock seconds for one run, or "-" if it would not run
    local t
    t=$( { time "$1" >/dev/null 2>&1; } 2>&1 ) || { echo "-"; return; }
    echo "$t"
}

printf '%-10s' program
for t in $targets; do printf '%18s' "$t"; done
printf '\n'

for src in part-b/*.c; do
    p=$(basename "$src" .c)
    base=$(seconds "bin/$p-$native")
    printf '%-10s' "$p"
    for t in $targets; do
        if [ "$t" = "$native" ]; then s=$base; else s=$(seconds "bin/$p-$t"); fi
        if [ "$s" = "-" ] || [ "$base" = "-" ]; then
            printf '%18s' "$s"
        else
            printf '%10ss %5sx' "$s" "$(awk -v a="$s" -v b="$base" 'BEGIN { printf "%.1f", a / b }')"
        fi
    done
    printf '\n'
done
echo
echo "Times include start-up; speed also includes its 1 s of sleep."
