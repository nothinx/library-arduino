// HC-SR04: berapa lama satu pengukuran memblokir saat sensor lepas (echo LOW terus) dan saat
// echo macet HIGH, di simavr tanpa sensor. Pilih dengan -DLIB=n.
// 1 = NewPing 1.9.7, 2 = Ultrasonic 3.0.0 (ErickSimoes), 3 = HCSR04 2.0.0 (Martinsos), 4 = HCSR04 2.0.3 (gamegine).
// Timer0 tetap hidup (micros() dipakai pustaka untuk timeout). Waktu diukur Timer1 prescaler 64
// (4 µs per tik, rentang 262 ms tanpa overflow, tetap benar walau pustaka mematikan interrupt).
#include <avr/sleep.h>
const uint8_t TRIG = 4, ECHO = 5;
volatile float o;
#if LIB == 1
#include <NewPing.h>
NewPing s(TRIG, ECHO, 400);
#define UKURJARAK() (o = s.ping_cm())
#elif LIB == 2
#include <Ultrasonic.h>
Ultrasonic s(TRIG, ECHO);
#define UKURJARAK() (o = s.read())
#elif LIB == 3
#include <HCSR04.h>
UltraSonicDistanceSensor s(TRIG, ECHO);
#define UKURJARAK() (o = s.measureDistanceCm())
#elif LIB == 4
#include <HCSR04.h>
HCSR04 s(TRIG, ECHO);
#define UKURJARAK() (o = s.dist())
#endif
void ukur(const __FlashStringHelper *nama) {
  TCCR1A = 0; TCCR1B = 0; TCNT1 = 0; TCCR1B = _BV(CS11) | _BV(CS10); // /64
  UKURJARAK();
  uint16_t t = TCNT1; TCCR1B = 0;
  Serial.print(F("BENCH ")); Serial.print(nama); Serial.print(' '); Serial.println((unsigned long)t * 4);
  Serial.flush();
}
void setup() {
  Serial.begin(115200);
  ukur(F("lepas_echo_low_us"));
  pinMode(ECHO, INPUT_PULLUP); // echo terbaca HIGH terus = sensor macet
  ukur(F("macet_echo_high_us"));
  Serial.print(F("BENCH sizeof ")); Serial.println(sizeof(s));
  Serial.println(F("BENCH_SELESAI")); Serial.flush();
  cli(); set_sleep_mode(SLEEP_MODE_PWR_DOWN); sleep_enable(); sleep_cpu();
}
void loop() {}
