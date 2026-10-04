#include "Siklus.h"
volatile float a = 1.5f, b = 2.25f, r; volatile long x = 12345, y = 67, q;
void setup() {
  Serial.begin(115200);
  UKUR("kosong", 1000, asm volatile(""));
  UKUR("float_kali", 1000, r = a * b);
  UKUR("float_bagi", 1000, r = a / b);
  UKUR("long_bagi", 1000, q = x / y);
  UKUR("sin", 100, r = sin(a));
  selesai();
}
void loop() {}
