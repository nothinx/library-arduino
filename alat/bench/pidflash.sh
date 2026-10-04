#!/bin/bash
# pidflash.sh <folder-KontrolPID>: flash & RAM sketch pidflash untuk PILIH=0..5
B=$(cd "$(dirname "$0")" && pwd); P=$B/pesaing
for n in 0 1 2 3 4 5; do printf "PILIH $n "; arduino-cli compile --clean -b arduino:avr:uno --build-property "compiler.cpp.extra_flags=-DPILIH=$n" --library "$1" --library $P/PID --library $P/QuickPID --library $P/FastPID $B/banding/pidflash </dev/null 2>&1 | sed -n 's/.*Sketch uses \([0-9]*\) bytes.*/flash \1/p;s/.*Global variables use \([0-9]*\) bytes.*/ram \1/p' | tr '\n' ' '; echo; done
