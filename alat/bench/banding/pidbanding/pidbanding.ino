#include "Siklus.h"
#include <KontrolPID.h>
#include <PID_v1.h>
#include <QuickPID.h>
#include <FastPID.h>
extern volatile unsigned long timer0_millis;
// Nilai sensor berganti tiap panggilan (8 nilai dekat target) agar tidak mentok.
static const float NF[8] = {95.0f, 97.5f, 99.0f, 100.5f, 101.0f, 99.5f, 98.0f, 96.5f};
static const int16_t NI[8] = {950, 975, 990, 1005, 1010, 995, 980, 965};
volatile float vt = 100, vo; volatile int16_t vti = 1000, voi;
KontrolPID kami(2, 0.5f, 0.1f), kamiF(2, 0.5f, 0.1f);
double v1In, v1Out, v1Sp = 100;
PID v1(&v1In, &v1Out, &v1Sp, 2, 0.5, 0.1, DIRECT);
float qIn, qOut, qSp = 100;
QuickPID qp(&qIn, &qOut, &qSp, 2, 0.5f, 0.1f, QuickPID::Action::direct);
FastPID fp(2, 0.5f, 0.1f, 100, 16, true);
void setup() {
  Serial.begin(115200);
  kami.aturBatas(0, 255); kamiF.aturBatas(0, 255); kamiF.aturFilterD(0.05f);
  v1.SetSampleTime(10); v1.SetMode(AUTOMATIC);
  qp.SetSampleTimeUs(10000); qp.SetMode(QuickPID::Control::timer);
  fp.setOutputRange(0, 255);
  UKUR("kosong", 1000, vo = NF[_i & 7]);
  UKUR("kami_dt", 1000, vo = kami.hitung(vt, NF[_i & 7], 0.01f));
  UKUR("kami_micros", 1000, vo = kami.hitung(vt, NF[_i & 7]));
  UKUR("kami_dt_filterD", 1000, vo = kamiF.hitung(vt, NF[_i & 7], 0.01f));
  UKUR("millis_maju", 1000, timer0_millis += 10; vo = NF[_i & 7]);
  UKUR("pidv1_compute", 1000, timer0_millis += 10; v1In = NF[_i & 7]; v1.Compute(); vo = v1Out);
  UKUR("quickpid_compute", 1000, qIn = NF[_i & 7]; qp.Compute(); vo = qOut);
  UKUR("kosong_int", 1000, voi = NI[_i & 7]);
  UKUR("fastpid_step", 1000, voi = fp.step(vti, NI[_i & 7]));
  Serial.print(F("BENCH sizeof_kami ")); Serial.println(sizeof(KontrolPID));
  Serial.print(F("BENCH sizeof_pidv1 ")); Serial.println(sizeof(PID));
  Serial.print(F("BENCH sizeof_quickpid ")); Serial.println(sizeof(QuickPID));
  Serial.print(F("BENCH sizeof_fastpid ")); Serial.println(sizeof(FastPID));
  selesai();
}
void loop() {}
