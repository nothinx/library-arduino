// LogikaFuzzy, sistem dan bentuk sketch sama dengan eflKipas.
#include "Siklus.h"
#include <LogikaFuzzy.h>
LogikaFuzzy<1, 1, 3, 3> f;
volatile float hasil, masuk = 25;
void setup() {
  Serial.begin(115200);
  uint8_t suhu = f.tambahMasukan(0, 50), kipas = f.tambahKeluaran(0, 255);
  uint8_t ma[3] = {f.tambahHimpunan(suhu, trapesium(0, 0, 20, 27)), f.tambahHimpunan(suhu, segitiga(22, 28, 34)), f.tambahHimpunan(suhu, trapesium(30, 37, 50, 50))};
  uint8_t mk[3] = {f.tambahHimpunan(kipas, trapesium(0, 0, 30, 90)), f.tambahHimpunan(kipas, segitiga(60, 140, 220)), f.tambahHimpunan(kipas, trapesium(180, 230, 255, 255))};
  for (int i = 0; i < 3; i++) f.jika(ma[i]).maka(mk[i]);
  f.masukan(suhu, masuk);
  UKUR("lf_25C", 20, { f.hitung(); hasil = f.keluaran(kipas); });
  f.masukan(suhu, 10);
  UKUR("lf_10C", 20, { f.hitung(); hasil = f.keluaran(kipas); });
  f.aturResolusi(21);
  f.masukan(suhu, masuk);
  UKUR("lf_25C_res21", 20, { f.hitung(); hasil = f.keluaran(kipas); });
  Serial.print(F("BENCH lf_hasil_25C_res21 ")); Serial.println(hasil, 4);
  selesai();
}
void loop() {}
