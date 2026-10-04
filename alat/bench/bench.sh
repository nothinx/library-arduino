#!/bin/bash
# bench.sh <folder-sketch> [--library <folder>]...
# Compile sketch untuk Uno, jalankan di simavr (ATmega328P 16 MHz), cetak:
#   FLASH <byte> / RAM <byte> / BENCH <nama> <siklus per panggilan>
# Sketch harus #include "Siklus.h" (salin dari folder bench/ukur) dan memanggil selesai() di akhir setup().
set -e
dir="$1"; shift
name=$(basename "$dir")
out="$(cd "$(dirname "$0")" && pwd)/out/$name.$$"
mkdir -p "$out"
cp -n "$(dirname "$0")/ukur/Siklus.h" "$dir/" 2>/dev/null || true
log=$(arduino-cli compile --clean -b arduino:avr:uno "$@" --output-dir "$out" "$dir" </dev/null 2>&1) || { echo "$log" | grep -i error | head -5; exit 1; }
echo "$log" | sed -n 's/.*Sketch uses \([0-9]*\) bytes.*/FLASH \1/p; s/.*Global variables use \([0-9]*\) bytes.*/RAM \1/p'
MSYS_NO_PATHCONV=1 docker run --rm -v "$(cygpath -w "$out"):/w" simavr timeout 600 simavr -m atmega328p -f 16000000 "/w/$name.ino.elf" 2>&1 \
  | sed 's/\x1b\[[0-9;]*m//g; s/\.\.$//' | grep -a "^BENCH" | grep -v BENCH_SELESAI
rm -rf "$out"
