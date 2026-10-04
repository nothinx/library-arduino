#include <currency.h>
#include "Siklus.h"
struct Buang : Print { size_t write(uint8_t) override { return 1; } } buang;
volatile int32_t k32 = 125, s32 = 1250000, b32 = 2147483647;
volatile int64_t r64 = -9223372036854775807LL, k64 = 125, s64 = 1250000, b64 = 2147483647;
volatile size_t hasil;
void setup() {
  Serial.begin(115200);
  UKUR("currency_125", 100, hasil = currency(k32, 0, ',', '.', 'R')[0]);
  UKUR("currency_1250000", 100, hasil = currency(s32, 0, ',', '.', 'R')[0]);
  UKUR("currency_2147483647", 100, hasil = currency(b32, 0, ',', '.', 'R')[0]);
  UKUR("currency64_125", 50, hasil = currency64(k64, 0, ',', '.', 'R')[0]);
  UKUR("currency64_1250000", 50, hasil = currency64(s64, 0, ',', '.', 'R')[0]);
  UKUR("currency64_2147483647", 50, hasil = currency64(b64, 0, ',', '.', 'R')[0]);
  UKUR("currency64_int64", 20, hasil = currency64(r64, 0, ',', '.', 'R')[0]);
  UKUR("print_long_125", 100, hasil = buang.print((long)k32));
  UKUR("print_long_1250000", 100, hasil = buang.print((long)s32));
  UKUR("print_long_2147483647", 100, hasil = buang.print((long)b32));
  selesai();
}
void loop() {}
