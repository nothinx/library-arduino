// Pembanding state machine: dua tahap A <-> B, pindah setelah 1000 ms. Pilih dengan -DLIB=n.
// "tunggu": belum waktunya pindah (millis beku). "pindah": millis maju 1000 ms tiap panggilan.
#include "Siklus.h"
extern volatile unsigned long timer0_millis;
volatile uint8_t n;
void kosong() {}
#if LIB == 0
#include <AlurProgram.h>
#define NAMA "AlurProgram"
enum { A, B };
AlurProgram alur(A);
void LANGKAH() {
  switch (alur.tahap()) {
    case A: if (alur.baruMasuk()) n++; alur.pindahSetelah(B, 1000); break;
    case B: if (alur.baruMasuk()) n++; alur.pindahSetelah(A, 1000); break;
  }
}
#define UKURAN sizeof(alur)
#elif LIB == 1
#include <Fsm.h>
#define NAMA "arduino-fsm"
void masuk() { n++; }
State sa(masuk, kosong, kosong), sb(masuk, kosong, kosong);
Fsm fsm(&sa);
void LANGKAH() { fsm.run_machine(); }
#define SIAP() (fsm.add_timed_transition(&sa, &sb, 1000, NULL), fsm.add_timed_transition(&sb, &sa, 1000, NULL))
#define UKURAN (sizeof(fsm) + 2 * sizeof(State))
#elif LIB == 2
#include <SimpleFSM.h>
#define NAMA "SimpleFSM"
void masuk() { n++; }
SimpleFSM fsm;
State sa("A", masuk), sb("B", masuk);
TimedTransition tt[] = {TimedTransition(&sa, &sb, 1000), TimedTransition(&sb, &sa, 1000)};
void LANGKAH() { fsm.run(0); }
#define SIAP() (fsm.add(tt, 2), fsm.setInitialState(&sa))
#define UKURAN (sizeof(fsm) + 2 * sizeof(State) + sizeof(tt))
#elif LIB == 3
#include <StateMachine.h>
#define NAMA "StateMachine"
StateMachine m;
unsigned long t0;
void fa() { if (m.executeOnce) { n++; t0 = millis(); } }
void fb() { if (m.executeOnce) { n++; t0 = millis(); } }
bool habis() { return millis() - t0 >= 1000; }
State *sa = m.addState(fa), *sb = m.addState(fb);
void LANGKAH() { m.run(); }
#define SIAP() (sa->addTransition(habis, sb), sb->addTransition(habis, sa))
#define UKURAN (sizeof(m) + 2 * sizeof(State))
#elif LIB == 4
#include <yasm.h>
#define NAMA "YASM"
YASM sm;
void fb();
void fa() { if (sm.isFirstRun()) n++; if (sm.elapsed(1000)) sm.next(fb); }
void fb() { if (sm.isFirstRun()) n++; if (sm.elapsed(1000)) sm.next(fa); }
void LANGKAH() { sm.run(); }
#define SIAP() sm.next(fa)
#define UKURAN sizeof(sm)
#endif
#ifndef SIAP
#define SIAP()
#endif
void setup() {
  Serial.begin(115200);
  SIAP();
  for (int i = 0; i < 5; i++) LANGKAH();
  UKUR("tunggu", 1000, LANGKAH());
  UKUR("millis_maju", 1000, timer0_millis += 1000);
  UKUR("pindah", 1000, { timer0_millis += 1000; LANGKAH(); });
  Serial.print(F("BENCH ram ")); Serial.println(UKURAN);
  Serial.print(F("BENCH masuk ")); Serial.println(n);
  selesai();
}
void loop() {}
