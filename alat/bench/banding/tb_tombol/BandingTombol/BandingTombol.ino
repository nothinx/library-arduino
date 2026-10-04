// Pembanding pustaka tombol. Pilih dengan -DLIB=n.
#include "Siklus.h"
const uint8_t PIN = 2;
volatile bool hasil, masukan;
void cb() {}
#if LIB == 0
#include <TombolPintar.h>
TombolPintar b(PIN);
#define NAMA "TombolPintar"
#define MULAI() b.mulai()
#define TICK() (hasil = b.perbarui())
#define TICKB() (hasil = b.perbarui(masukan))
#elif LIB == 1
#include <OneButton.h>
OneButton b(PIN, true, true);
#define NAMA "OneButton"
#define MULAI() (b.attachClick(cb), b.attachDoubleClick(cb), b.attachLongPressStart(cb), b.attachDuringLongPress(cb))
#define TICK() b.tick()
#define TICKB() b.tick(masukan)
#elif LIB == 2
#include <Button2.h>
Button2 b;
#define NAMA "Button2"
#define MULAI() (b.begin(PIN), b.setClickHandler([](Button2&){}), b.setDoubleClickHandler([](Button2&){}), b.setLongClickDetectedHandler([](Button2&){}))
#define TICK() b.loop()
#elif LIB == 3
#include <EasyButton.h>
EasyButton b(PIN, 20);
#define NAMA "EasyButton"
#define MULAI() (b.begin(), b.onPressed(cb), b.onPressedFor(1000, cb))
#define TICK() (hasil = b.read())
#elif LIB == 4
#include <JC_Button.h>
Button b(PIN, 20);
#define NAMA "JC_Button"
#define MULAI() b.begin()
#define TICK() (hasil = b.read(), hasil = b.wasPressed() | b.pressedFor(1000))
#elif LIB == 5
#include <Bounce2.h>
Bounce2::Button b;
#define NAMA "Bounce2"
#define MULAI() (b.attach(PIN, INPUT_PULLUP), b.interval(20), b.setPressedState(LOW))
#define TICK() (hasil = b.update(), hasil = b.pressed())
#elif LIB == 6
#include <ezButton.h>
ezButton b(PIN);
#define NAMA "ezButton"
#define MULAI() b.setDebounceTime(20)
#define TICK() (b.loop(), hasil = b.isPressed())
#elif LIB == 7
#include <AceButton.h>
using namespace ace_button;
AceButton b(PIN);
#define NAMA "AceButton"
#define MULAI() do { pinMode(PIN, INPUT_PULLUP); b.init(PIN); ButtonConfig* c = b.getButtonConfig(); c->setEventHandler([](AceButton*, uint8_t, uint8_t) {}); \
  c->setFeature(ButtonConfig::kFeatureClick); c->setFeature(ButtonConfig::kFeatureDoubleClick); \
  c->setFeature(ButtonConfig::kFeatureLongPress); c->setFeature(ButtonConfig::kFeatureRepeatPress); } while (0)
#define TICK() b.check()
#endif

void tekan(bool t) { masukan = t; if (t) { pinMode(PIN, OUTPUT); digitalWrite(PIN, LOW); } else pinMode(PIN, INPUT_PULLUP); }
void jalankan(uint16_t ms) { uint32_t t = millis(); while (millis() - t < ms) TICK(); }

void setup() {
  Serial.begin(115200);
  MULAI();
  Serial.print(F("BENCH sizeof ")); Serial.println(sizeof(b));
  jalankan(500);
  UKUR("diam", 1000, TICK());
  tekan(true); jalankan(100);
  UKUR("ditahan", 1000, TICK());
  tekan(false); jalankan(50);
  UKUR("jeda_klik", 1000, TICK());
  jalankan(500); tekan(true); jalankan(1100);
  UKUR("berulang", 1000, TICK());
#ifdef TICKB
  tekan(false); jalankan(500);
  UKUR("bool_diam", 1000, TICKB());
#endif
  selesai();
}
void loop() {}
