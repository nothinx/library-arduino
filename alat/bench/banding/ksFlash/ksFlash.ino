#include "Siklus.h"
#include <KalibrasiSensor.h>
const float f8[8] PROGMEM = {4.2, 4.0, 3.8, 3.7, 3.6, 3.5, 3.3, 3.0}, g8[8] PROGMEM = {100, 85, 60, 40, 20, 10, 5, 0};
const float m8[8] = {4.2, 4.0, 3.8, 3.7, 3.6, 3.5, 3.3, 3.0}, n8[8] = {100, 85, 60, 40, 20, 10, 5, 0};
KalibrasiSensor kf(f8, g8, 8, KalibrasiSensor::DI_FLASH), kr(m8, n8, 8);
void setup() {
  Serial.begin(115200);
  bool sama = kf.valid();
  for (float x = 2.5; x < 4.5; x += 0.01) sama &= kf.ubah(x) == kr.ubah(x);
  Serial.print(F("BENCH flash_sama_ram ")); Serial.println(sama);
  Serial.print(F("BENCH ubah_3.65 ")); Serial.println(kf.ubah(3.65), 4);
  selesai();
}
void loop() {}
