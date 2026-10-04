#include <KalibrasiSensor.h>
const float A[] = {1, 2, 3}; const float B[] PROGMEM = {1, 2, 3};
KalibrasiSensor k(A, A, 3), f(B, B, 3, KalibrasiSensor::DI_FLASH);
volatile float v;
void setup() { k.aturEkstrapolasi(v > 0); }
void loop() { v = k.ubah(v) + f.ubah(v) + k.mulai() + k.valid(); }
