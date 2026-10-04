#!/bin/bash
# flash_fs.sh <folder-FilterSensor>: flash & RAM sketch satu filter, dikurangi sketch kosong setara
B=$(cd "$(dirname "$0")" && pwd); P=$B/pesaing; L=$1
declare -A V=(
 [kosong]='out = in;'
 [rata8]='static RataRataBergerak<8> f; out = f.saring(in);'
 [med5]='static FilterMedian<5> f; out = f.saring(in);'
 [ema]='static FilterEMA f(0.2f); out = f.saring(in);'
 [kalman]='static FilterKalman f(5, 1); out = f.saring(in);'
 [RA8]='static RunningAverage f(8); f.add(in); out = f.getFastAverage();'
 [RM5]='static RunningMedian f(5); f.add(in); out = f.getMedian();'
 [SKF]='static SimpleKalmanFilter f(5, 5, 0.01f); out = f.updateEstimate(in);'
 [Ewma]='static Ewma f(0.2); out = f.filter(in);'
)
for k in kosong rata8 med5 ema kalman RA8 RM5 SKF Ewma; do
  d=$B/out/fs_$k; mkdir -p $d
  printf '#include <FilterSensor.h>\n#include <RunningAverage.h>\n#include <RunningMedian.h>\n#include <SimpleKalmanFilter.h>\n#include <Ewma.h>\nvolatile float in, out;\nvoid setup() {}\nvoid loop() { %s }\n' "${V[$k]}" > $d/fs_$k.ino
  arduino-cli compile --clean -b arduino:avr:uno --library "$L" --library $P/RunningAverage --library $P/RunningMedian --library $P/SimpleKalmanFilter --library $P/EWMA $d </dev/null 2>&1 | sed -n "s/.*Sketch uses \([0-9]*\) bytes.*/$k FLASH \1/p; s/.*Global variables use \([0-9]*\) bytes.*/RAM \1/p" | tr '\n' ' '; echo
done
