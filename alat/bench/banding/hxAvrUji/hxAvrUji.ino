// Uji jalur register port AVR TimbanganHX711 di simavr: HX711 tiruan dari interrupt pin-change.
// SCK = pin 2 (PD2, PCINT18), DT = pin 3 (PD3, dijadikan OUTPUT oleh tiruan).
// Setiap pulsa SCK (naik+turun saat interrupt mati = satu ISR) mengeluarkan bit berikutnya, MSB dulu;
// setelah 24 bit DT HIGH (datasheet hlm. 4).
#include "Siklus.h"
#include <TimbanganHX711.h>
TimbanganHX711 t(3, 2);
volatile long nilai = -123457; // 0xFE1DBF
volatile uint8_t pulsa = 0;
ISR(PCINT2_vect) {
  if (PIND & _BV(2)) return; // hanya hitung saat SCK sudah turun
  pulsa++;
  if (pulsa <= 24) { if ((nilai >> (24 - pulsa)) & 1) PORTD |= _BV(3); else PORTD &= ~_BV(3); }
  else PORTD |= _BV(3);
}
bool satuKonversi() { pulsa = 0; PORTD &= ~_BV(3); return t.perbarui(); } // DT LOW = data siap
void setup() {
  Serial.begin(115200);
  t.mulai();
  DDRD |= _BV(3); PORTD |= _BV(3);
  PCICR |= _BV(PCIE2); PCMSK2 |= _BV(PCINT18);
  for (int i = 0; i < 5; i++) satuKonversi(); // settling
  bool ok = satuKonversi();
  Serial.print(F("BENCH ok ")); Serial.println(ok);
  Serial.print(F("BENCH mentah ")); Serial.println(t.mentah());
  Serial.print(F("BENCH pulsa ")); Serial.println(pulsa);
  Serial.print(F("BENCH status ")); Serial.println((int)t.status());
  nilai = 8388000; satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi(); satuKonversi();
  Serial.print(F("BENCH mentah2 ")); Serial.println(t.mentah());
  t.aturGain(64);
  for (int i = 0; i < 7; i++) satuKonversi();
  Serial.print(F("BENCH pulsa_gain64 ")); Serial.println(pulsa);
  Serial.print(F("BENCH sizeof ")); Serial.println(sizeof(t));
  selesai();
}
void loop() {}
