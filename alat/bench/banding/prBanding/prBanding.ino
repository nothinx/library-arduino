// Odometri: PosisiRobot vs DeadReckoning-library 1.0.0. Roda 6,5 cm, jarak roda 15 cm, 360 pulsa/putaran.
// Tiap panggilan: belok (kiri +3, kanan +2 pulsa), lurus (+3/+3), atau diam.
#include "Siklus.h"
#include <PosisiRobot.h>
#include <DeadReckoner.h>
PosisiRobot pos(6.5, 15, 360);
volatile unsigned long kiriU, kananU;
DeadReckoner dr(&kiriU, &kananU, 360, 3.25, 15);
long kiri, kanan;
volatile float o;
void setup() {
  Serial.begin(115200);
  pos.perbarui(0, 0);
  UKUR("pr_belok", 1000, { kiri += 3; kanan += 2; pos.perbarui(kiri, kanan); });
  UKUR("pr_lurus", 1000, { kiri += 3; kanan += 3; pos.perbarui(kiri, kanan); });
  UKUR("pr_diam", 1000, pos.perbarui(kiri, kanan));
  UKUR("pr_arah", 1000, { asm volatile("" ::: "memory"); o = pos.arah(); });
  UKUR("pr_selisihArahKe", 1000, { asm volatile("" ::: "memory"); o = pos.selisihArahKe(100, 50); });
  UKUR("dr_belok", 1000, { kiriU += 3; kananU += 2; dr.computePosition(); });
  UKUR("dr_diam", 1000, dr.computePosition());
  Serial.print(F("BENCH x_x100 ")); Serial.println((long)(pos.x() * 100));
  Serial.print(F("BENCH y_x100 ")); Serial.println((long)(pos.y() * 100));
  Serial.print(F("BENCH arah_x100 ")); Serial.println((long)(pos.arah() * 100));
  Serial.print(F("BENCH sizeof_pr ")); Serial.println(sizeof(pos));
  Serial.print(F("BENCH sizeof_dr ")); Serial.println(sizeof(dr));
  selesai();
}
void loop() {}
