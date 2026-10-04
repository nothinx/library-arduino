// TanpaDelay vs NoDelay 2.2.0 vs Neotimer 1.1.6 vs arduino-timer 3.0.1, interval 10 ms.
// "tunggu": belum waktunya (paling sering). "kejadian": millis() dimajukan 10 ms tiap panggilan.
#include "Siklus.h"
#include <TanpaDelay.h>
#include <NoDelay.h>
#include <neotimer.h>
#include <arduino-timer.h>
extern volatile unsigned long timer0_millis;
TanpaDelay td(10);
noDelay nd(10);
Neotimer nt(10);
Timer<1> at;
volatile bool hasil;
volatile uint8_t n;
bool tugas(void *) { n++; return true; }
void setup() {
  Serial.begin(115200);
  nt.start();
  at.every(10, tugas);
  td.mulai(); nd.start(); nt.start();
  UKUR("kosong", 1000, hasil = n);
  UKUR("td_tunggu", 1000, hasil = td.waktunya());
  UKUR("nd_tunggu", 1000, hasil = nd.update());
  UKUR("nt_tunggu", 1000, hasil = nt.repeat());
  UKUR("at_tunggu", 1000, at.tick());
  UKUR("millis_maju", 1000, timer0_millis += 10);
  UKUR("td_kejadian", 1000, { timer0_millis += 10; hasil = td.waktunya(); });
  UKUR("nd_kejadian", 1000, { timer0_millis += 10; hasil = nd.update(); });
  UKUR("nt_kejadian", 1000, { timer0_millis += 10; hasil = nt.repeat(); });
  UKUR("at_kejadian", 1000, { timer0_millis += 10; at.tick(); });
  Serial.print(F("BENCH sizeof_td ")); Serial.println(sizeof(td));
  Serial.print(F("BENCH sizeof_nd ")); Serial.println(sizeof(nd));
  Serial.print(F("BENCH sizeof_nt ")); Serial.println(sizeof(nt));
  Serial.print(F("BENCH sizeof_at ")); Serial.println(sizeof(at));
  selesai();
}
void loop() {}
