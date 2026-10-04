// Flash sketch sama: kirim "SOS" ke pin 13. PILIH 0 = kosong, 1 = SandiMorse, 2 = Etherkit, 3 = MorseEncoder.
#if PILIH == 1
#include <SandiMorse.h>
SandiMorse m(13, 20);
void setup() { m.mulai(); m.kirim("SOS"); }
void loop() { m.perbarui(); }
#elif PILIH == 2
#include <Morse.h>
Morse m(13, 20);
void setup() { m.send("SOS"); }
void loop() { m.update(); delay(1); }
#elif PILIH == 3
#include <MorseEncoder.h>
MorseEncoder m(13);
void setup() { m.beginLight(20); m.print("SOS"); }
void loop() {}
#else
void setup() { pinMode(13, OUTPUT); }
void loop() { digitalWrite(13, millis() & 1); delay(1); }
#endif
