// Perintah serial: PerintahSerial vs SerialCommands 2.2.0 vs SerialCmd 1.1.6. Pilih dengan -DLIB=n.
// Lima perintah (LED, MOTOR, SET, GET, HELP); baris "SET KP 2.5" dibaca dari Stream di memori,
// dengan akhiran baris yang dibutuhkan tiap library. "diam": tidak ada data.
#include "Siklus.h"
struct Pita : Stream {
  const char *p = "";
  int available() override { return *p ? 1 : 0; }
  int read() override { return *p ? *p++ : -1; }
  int peek() override { return *p ? *p : -1; }
  size_t write(uint8_t) override { return 1; }
} pita;
volatile float kp;
volatile uint8_t n;
#if LIB == 0
#include <PerintahSerial.h>
#define BARIS "SET KP 2.5\n"
PerintahSerial cmd(pita);
void SIAP() {}
void TICK() {
  if (!cmd.ada()) return;
  if (cmd.adalah(F("LED"))) n++;
  else if (cmd.adalah(F("MOTOR"))) n++;
  else if (cmd.adalah(F("SET"))) { if (cmd.adalah(F("KP"), 1)) kp = cmd.desimal(2); }
  else if (cmd.adalah(F("GET"))) n++;
  else if (cmd.adalah(F("HELP"))) n++;
}
#define UKURAN sizeof(cmd)
#elif LIB == 1
#include <SerialCommands.h>
#define BARIS "SET KP 2.5\r\n"
char buf[64];
SerialCommands cmd(&pita, buf, sizeof(buf), "\r\n", " ");
void hN(SerialCommands *) { n++; }
void hSet(SerialCommands *s) { char *a = s->Next(); if (a && !strcmp(a, "KP")) { char *b = s->Next(); if (b) kp = atof(b); } }
SerialCommand c1("LED", hN), c2("MOTOR", hN), c3("SET", hSet), c4("GET", hN), c5("HELP", hN);
void SIAP() { cmd.AddCommand(&c1); cmd.AddCommand(&c2); cmd.AddCommand(&c3); cmd.AddCommand(&c4); cmd.AddCommand(&c5); }
void TICK() { cmd.ReadSerial(); }
#define UKURAN (sizeof(cmd) + sizeof(buf) + 5 * sizeof(SerialCommand))
#elif LIB == 2
#include <SerialCmd.h>
#define BARIS "SET KP 2.5\r"
SerialCmd cmd(pita, SERIALCMD_CR, (char *)SERIALCMD_SPACE);
void hN() { n++; }
void hSet() { char *a = cmd.ReadNext(); if (a && !strcmp(a, "KP")) { char *b = cmd.ReadNext(); if (b) kp = atof(b); } }
void SIAP() { cmd.AddCmd("LED", SERIALCMD_FROMALL, hN); cmd.AddCmd("MOTOR", SERIALCMD_FROMALL, hN); cmd.AddCmd("SET", SERIALCMD_FROMALL, hSet); cmd.AddCmd("GET", SERIALCMD_FROMALL, hN); cmd.AddCmd("HELP", SERIALCMD_FROMALL, hN); }
void TICK() { cmd.ReadSer(); }
#define UKURAN sizeof(cmd)
#endif
void setup() {
  Serial.begin(115200);
  SIAP();
  UKUR("diam", 1000, TICK());
  UKUR("baris_SET_KP", 100, { pita.p = BARIS; TICK(); });
  Serial.print(F("BENCH kp_x10 ")); Serial.println((int)(kp * 10));
  Serial.print(F("BENCH ram ")); Serial.println(UKURAN);
  selesai();
}
void loop() {}
