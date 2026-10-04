// Flash tambahan: PILIH 0 = kosong, 1 = KontrolPID dt, 2 = KontrolPID micros, 3 = PID_v1, 4 = QuickPID, 5 = FastPID
#include <KontrolPID.h>
#include <PID_v1.h>
#include <QuickPID.h>
#include <FastPID.h>
volatile float vt = 100, vn = 95, vo; volatile int16_t vti = 1000, vni = 950, voi;
#if PILIH == 1 || PILIH == 2
KontrolPID pid(2, 0.5f, 0.1f);
#elif PILIH == 3
double in, out, sp;
PID pid(&in, &out, &sp, 2, 0.5, 0.1, DIRECT);
#elif PILIH == 4
float in, out, sp;
QuickPID pid(&in, &out, &sp, 2, 0.5f, 0.1f, QuickPID::Action::direct);
#elif PILIH == 5
FastPID pid(2, 0.5f, 0.1f, 100, 16, true);
#endif
void setup() {
#if PILIH == 1 || PILIH == 2
  pid.aturBatas(0, 255);
#elif PILIH == 3 || PILIH == 4
  pid.SetOutputLimits(0, 255); pid.SetMode(1);
#elif PILIH == 5
  pid.setOutputRange(0, 255);
#endif
}
void loop() {
#if PILIH == 0
  vo = vt - vn; voi = vti - vni;
#elif PILIH == 1
  vo = pid.hitung(vt, vn, 0.01f);
#elif PILIH == 2
  vo = pid.hitung(vt, vn);
#elif PILIH == 3 || PILIH == 4
  sp = vt; in = vn; pid.Compute(); vo = out;
#elif PILIH == 5
  voi = pid.step(vti, vni);
#endif
}
