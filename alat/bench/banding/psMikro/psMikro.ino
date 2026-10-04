#include "Siklus.h"
#include <PerintahSerial.h>
struct Pita : Stream {
  const char *p = "";
  int available() override { return *p ? 1 : 0; }
  int read() override { return *p ? *p++ : -1; }
  int peek() override { return *p ? *p : -1; }
  size_t write(uint8_t) override { return 1; }
} pita;
PerintahSerial cmd(pita);
volatile bool b; volatile float kp; volatile char cc;
void setup() {
  Serial.begin(115200);
  UKUR("baca_pita_saja", 100, { pita.p = "SET KP 2.5\n"; while (pita.available()) cc = pita.read(); });
  UKUR("ada", 100, { pita.p = "SET KP 2.5\n"; b = cmd.ada(); });
  UKUR("adalah_LED", 100, b = cmd.adalah(F("LED")));
  UKUR("adalah_SET", 100, b = cmd.adalah(F("SET")));
  UKUR("adalah_KP_1", 100, b = cmd.adalah(F("KP"), 1));
  UKUR("desimal_2", 100, kp = cmd.desimal(2));
  UKUR("atof", 100, kp = atof("2.5"));
  selesai();
}
void loop() {}
