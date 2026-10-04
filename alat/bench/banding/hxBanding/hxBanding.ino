// HX711: TimbanganHX711 vs bogde HX711 0.7.5 vs olkal HX711_ADC 1.2.12 vs RobTillaart HX711 0.6.5.
// simavr tidak punya HX711: "siap" dipicu dengan DT dipaksa LOW (OUTPUT LOW), "belum siap" dengan DT HIGH.
// Yang diukur: satu bacaan 24 bit penuh, dan pemeriksaan saat data belum siap. Pilih dengan -DLIB=n.
#include "Siklus.h"
const uint8_t PIN_DT = 3, PIN_SCK = 4;
volatile long o;
void dtLow() { pinMode(PIN_DT, OUTPUT); digitalWrite(PIN_DT, LOW); }
void dtHigh() { pinMode(PIN_DT, OUTPUT); digitalWrite(PIN_DT, HIGH); }
#if LIB == 0
#include <TimbanganHX711.h>
TimbanganHX711 s(PIN_DT, PIN_SCK);
#define SIAP() s.mulai()
#define BELUM() (o = s.perbarui())
#define BACA() (o = s.perbarui())
#elif LIB == 1
#include <HX711.h>
HX711 s;
#define SIAP() s.begin(PIN_DT, PIN_SCK)
#define BELUM() (o = s.is_ready())
#define BACA() (o = s.read())
#elif LIB == 2
#include <HX711_ADC.h>
HX711_ADC s(PIN_DT, PIN_SCK);
#define SIAP() s.begin()
#define BELUM() (o = s.update())
#define BACA() (o = s.update())
#elif LIB == 3
#include <HX711.h>
HX711 s;
#define SIAP() s.begin(PIN_DT, PIN_SCK, false, false) // doReset=true: read() menunggu selamanya tanpa sensor
#define BELUM() (o = s.is_ready())
#define BACA() (o = s.read())
#endif
void setup() {
  Serial.begin(115200);
  SIAP();
  dtHigh();
  UKUR("belum_siap", 1000, BELUM());
  dtLow();
  UKUR("baca_penuh", 100, BACA());
  UKUR("digitalWrite_HL", 1000, { digitalWrite(PIN_SCK, HIGH); digitalWrite(PIN_SCK, LOW); });
  Serial.print(F("BENCH sizeof ")); Serial.println(sizeof(s));
  selesai();
}
void loop() {}
