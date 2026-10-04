// KalibrasiSensor vs MultiMap 0.4.0 vs InterpolationLib 1.0.2, tabel naik 2/8/32 titik.
#include "Siklus.h"
#include <KalibrasiSensor.h>
#include <MultiMap.h>
#include <InterpolationLib.h>
float M2[2] = {0, 1023}, N2[2] = {0, 100};
float M8[8] = {3.0, 3.3, 3.5, 3.6, 3.7, 3.8, 4.0, 4.2}, N8[8] = {0, 5, 10, 20, 40, 60, 85, 100};
float M32[32], N32[32];
const float F8[8] PROGMEM = {3.0, 3.3, 3.5, 3.6, 3.7, 3.8, 4.0, 4.2}, G8[8] PROGMEM = {0, 5, 10, 20, 40, 60, 85, 100};
double D8[8] = {3.0, 3.3, 3.5, 3.6, 3.7, 3.8, 4.0, 4.2}, E8[8] = {0, 5, 10, 20, 40, 60, 85, 100};
double D32[32], E32[32];
KalibrasiSensor k2(M2, N2, 2), k8(M8, N8, 8), k32(M32, N32, 32), kf8(F8, G8, 8, KalibrasiSensor::DI_FLASH);
// Masukan bergilir di seluruh rentang agar semua ruas terkena.
static float X8[8] = {3.05, 3.4, 3.55, 3.65, 3.75, 3.9, 4.1, 4.15}, X32[8], X2[8] = {10, 150, 300, 450, 600, 750, 900, 1000};
static double DX8[8], DX32[8];
volatile float o;
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 32; i++) { M32[i] = i * 32; N32[i] = sqrt(i * 32.0); D32[i] = M32[i]; E32[i] = N32[i]; }
  for (int i = 0; i < 8; i++) { X32[i] = 16 + i * 128; DX8[i] = X8[i]; DX32[i] = X32[i]; }
  k2.mulai(); k8.mulai(); k32.mulai(); kf8.mulai();
  UKUR("kosong", 1000, o = X8[_i & 7]);
  UKUR("ks_2", 1000, o = k2.ubah(X2[_i & 7]));
  UKUR("ks_8", 1000, o = k8.ubah(X8[_i & 7]));
  UKUR("ks_8_flash", 1000, o = kf8.ubah(X8[_i & 7]));
  UKUR("ks_32", 1000, o = k32.ubah(X32[_i & 7]));
  UKUR("mm_2", 1000, o = multiMap<float>(X2[_i & 7], M2, N2, 2));
  UKUR("mm_8", 1000, o = multiMap<float>(X8[_i & 7], M8, N8, 8));
  UKUR("mm_32", 1000, o = multiMap<float>(X32[_i & 7], M32, N32, 32));
  UKUR("mmBS_32", 1000, o = multiMapBS<float>(X32[_i & 7], M32, N32, 32));
  UKUR("il_8", 1000, o = Interpolation::Linear(D8, E8, 8, DX8[_i & 7], true));
  UKUR("il_32", 1000, o = Interpolation::Linear(D32, E32, 32, DX32[_i & 7], true));
  Serial.print(F("BENCH sizeof ")); Serial.println(sizeof(KalibrasiSensor));
  selesai();
}
void loop() {}
