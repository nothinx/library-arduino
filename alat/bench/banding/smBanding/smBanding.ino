// Pengirim Morse: SandiMorse vs Etherkit Morse 1.1.2, 20 WPM, kirim "SOS".
// "tunggu": di tengah simbol (millis beku). Pilih dengan -DLIB=n.
#include "Siklus.h"
volatile bool hasil;
#if LIB == 0
#include <SandiMorse.h>
SandiMorse m(13, 20);
#define SIAP() (m.mulai(), m.kirim("SOS"))
#define TICK() (hasil = m.perbarui())
#define UKURAN sizeof(m)
#elif LIB == 1
#include <Morse.h>
Morse m(13, 20);
#define SIAP() m.send("SOS")
#define TICK() m.update()
#define UKURAN sizeof(m)
#elif LIB == 2
#include <MorseEncoder.h>
MorseEncoder m(13);
#define SIAP() m.beginLight(20)
#define TICK()
#define UKURAN sizeof(m)
#endif
void setup() {
  Serial.begin(115200);
  SIAP();
  TICK();
  UKUR("tunggu", 1000, TICK());
  Serial.print(F("BENCH ram ")); Serial.println(UKURAN);
#if LIB == 2
  uint32_t t = millis(); m.print("SOS"); Serial.print(F("BENCH blok_ms ")); Serial.println(millis() - t);
#endif
  selesai();
}
void loop() {}
