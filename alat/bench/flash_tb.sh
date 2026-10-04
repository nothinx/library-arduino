#!/bin/bash
# flash_tb.sh <folder-library-Terbilang>: flash & RAM tiap fungsi (dikurangi sketch kosong setara)
B=$(dirname "$0"); L=$1
declare -A V=(
 [kosong]='hasil = buang.write(n8);'
 [cetakTerbilang]='hasil = cetakTerbilang(buang, n64);'
 [terbilang_buf]='hasil = terbilang(buf, sizeof(buf), n64);'
 [formatAngka]='hasil = formatAngka(n64).teks[0];'
 [formatRupiah]='hasil = formatRupiah(n64).teks[0];'
 [semua]='hasil = cetakTerbilang(buang, n64) + terbilang(buf, sizeof(buf), n64) + formatAngka(n64).teks[0] + formatRupiah(n64).teks[0];'
 [currency]='hasil = currency(n32, 0, 0x2C, 0x2E, 0x52)[0];'
 [currency64]='hasil = currency64(n64, 0, 0x2C, 0x2E, 0x52)[0];'
 [print_long]='hasil = buang.print(n32);'
)
for k in kosong cetakTerbilang terbilang_buf formatAngka formatRupiah semua currency currency64 print_long; do
  d=$B/out/fl_$k; mkdir -p $d
  cat > $d/fl_$k.ino <<EOT
#include <Terbilang.h>
#include <currency.h>
struct Buang : Print { size_t write(uint8_t) override { return 1; } } buang;
volatile int64_t n64 = 125; volatile int32_t n32 = 125; volatile uint8_t n8 = 1; volatile size_t hasil; char buf[200];
void setup() { ${V[$k]} }
void loop() {}
EOT
  arduino-cli compile --clean -b arduino:avr:uno --library "$L" --library $B/pesaing/currency $d </dev/null 2>&1 | sed -n "s/.*Sketch uses \([0-9]*\) bytes.*/$k FLASH \1/p; s/.*Global variables use \([0-9]*\) bytes.*/$k RAM \1/p" | tr '\n' ' '; echo
done
