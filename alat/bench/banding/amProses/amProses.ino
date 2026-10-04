// Biaya CPU ArahMPU6050 per sampel FIFO (proses()) tanpa sensor. Hanya untuk pengukuran:
// anggota private dibuka agar proses() bisa dipanggil langsung.
#include "Siklus.h"
#define private public
#include <ArahMPU6050.h>
#undef private
ArahMPU6050 s;
// 8 sampel gyro (big-endian X, Y, Z), sedang berputar ±30 dps.
uint8_t sampel[8][6];
volatile float o;
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) for (int a = 0; a < 3; a++) { int16_t v = (a == 2 ? -2000 : 40) + i * 37; sampel[i][2 * a] = v >> 8; sampel[i][2 * a + 1] = v; }
  UKUR("proses_berputar", 1000, s.proses(sampel[_i & 7], 1 / 65.5f));
  for (int i = 0; i < 8; i++) for (int a = 0; a < 6; a++) sampel[i][a] = 0;
  for (int i = 0; i < 200; i++) s.proses(sampel[0], 1 / 65.5f);
  UKUR("proses_diam", 1000, s.proses(sampel[_i & 7], 1 / 65.5f));
  UKUR("arah", 1000, { asm volatile("" ::: "memory"); o = s.arah(); });
  UKUR("selisihKe", 1000, { asm volatile("" ::: "memory"); o = s.selisihKe(123); });
  Serial.print(F("BENCH sizeof ")); Serial.println(sizeof(s));
  selesai();
}
void loop() {}
