// Uji jalur ISR JarakHCSR04 di AVR (simavr): echo di pin 2 (INT0) digerakkan sketch sendiri.
// INT0 juga terpicu oleh perubahan pin yang dijadikan OUTPUT, jadi ISR library menangkap tepi
// naik/turun seperti dari sensor. Jarak 50 cm pada 20 °C = 50 * 20000 / 343,42 = 2912 µs.
#include "Siklus.h"
#include <JarakHCSR04.h>
JarakHCSR04 s(4, 2); // TRIG 4, ECHO 2 (INT0)
uint32_t pulsaAsli; // lebar pulsa sebenarnya menurut Timer1 (siklus/16), termasuk ISR yang menyela
void echo(uint16_t us) {
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  delayMicroseconds(300);
  TCCR1A = 0; TCCR1B = 0; TCNT1 = 0; TCCR1B = _BV(CS11); // /8 = 0,5 �s per tik
  PORTD |= _BV(2);
  delayMicroseconds(us);
  PORTD &= ~_BV(2);
  pulsaAsli = TCNT1 / 2; TCCR1B = 0;
}
void setup() {
  Serial.begin(115200);
  s.mulai();
  Serial.print(F("BENCH interrupt ")); Serial.println(s.pakaiInterrupt());
  for (int i = 0; i < 3; i++) {
    delay(65);
    s.perbarui();           // kirim trigger
    echo(2912);             // pantulan 50 cm
    s.perbarui();           // proses hasil ISR
  }
  Serial.print(F("BENCH status ")); Serial.println((int)s.status());
  Serial.print(F("BENCH jarak_x100 ")); Serial.println((long)(s.jarakCm() * 100));
  Serial.print(F("BENCH sizeof ")); Serial.println(sizeof(s));
  Serial.print(F("BENCH pulsa_asli_us ")); Serial.println(pulsaAsli);
  Serial.print(F("BENCH jarak_dari_pulsa_asli_x100 ")); Serial.println((long)(pulsaAsli * 343.42f * 0.005f));
  pinMode(2, OUTPUT); digitalWrite(2, LOW);
  delay(65); s.perbarui();  // trigger, lalu tidak ada echo
  UKUR("perbarui_menunggu_echo", 1000, s.perbarui());
  selesai();
}
void loop() {}
