#include "Siklus.h"
volatile uint64_t a=125, b=1250000, c=9223372036854775807ULL, r; volatile uint32_t x=125, y=2147483647, z; volatile uint16_t h=999, hr;
void setup() {
  Serial.begin(115200);
  UKUR("d64_125", 100, r = a / 1000);
  UKUR("m64_125", 100, r = a % 1000);
  UKUR("d64_1250000", 100, r = b / 1000);
  UKUR("d64_big", 100, r = c / 1000);
  UKUR("m64_big", 100, r = c % 1000);
  UKUR("mul64", 100, r = a * 1000);
  UKUR("d32_125", 100, z = x / 1000);
  UKUR("d32_big", 100, z = y / 1000);
  UKUR("dm32_big", 100, { uint32_t q = y / 1000; z = q; hr = y - q * 1000; });
  UKUR("d16", 100, hr = h / 100);
  UKUR("d10_64big", 100, r = c / 10);
  selesai();
}
void loop() {}
