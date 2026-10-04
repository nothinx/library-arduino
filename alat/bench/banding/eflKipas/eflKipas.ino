// eFLL 1.5.1, sistem sama dengan contoh KipasOtomatis LogikaFuzzy.
#include "Siklus.h"
#include <Fuzzy.h>
Fuzzy *f = new Fuzzy();
volatile float hasil, masuk = 25;
void setup() {
  Serial.begin(115200);
  FuzzyInput *suhu = new FuzzyInput(1);
  FuzzySet *dingin = new FuzzySet(0, 0, 20, 27), *hangat = new FuzzySet(22, 28, 28, 34), *panas = new FuzzySet(30, 37, 50, 50);
  suhu->addFuzzySet(dingin); suhu->addFuzzySet(hangat); suhu->addFuzzySet(panas);
  f->addFuzzyInput(suhu);
  FuzzyOutput *kipas = new FuzzyOutput(1);
  FuzzySet *mati = new FuzzySet(0, 0, 30, 90), *sedang = new FuzzySet(60, 140, 140, 220), *kencang = new FuzzySet(180, 230, 255, 255);
  kipas->addFuzzySet(mati); kipas->addFuzzySet(sedang); kipas->addFuzzySet(kencang);
  f->addFuzzyOutput(kipas);
  FuzzySet *ma[3] = {dingin, hangat, panas}, *mk[3] = {mati, sedang, kencang};
  for (int i = 0; i < 3; i++) {
    FuzzyRuleAntecedent *a = new FuzzyRuleAntecedent(); a->joinSingle(ma[i]);
    FuzzyRuleConsequent *c = new FuzzyRuleConsequent(); c->addOutput(mk[i]);
    f->addFuzzyRule(new FuzzyRule(i + 1, a, c));
  }
  f->setInput(1, masuk);
  UKUR("efll_25C", 20, { f->fuzzify(); hasil = f->defuzzify(1); });
  Serial.print(F("BENCH efll_hasil_25C ")); Serial.println(hasil, 4);
  f->setInput(1, 10);
  UKUR("efll_10C", 20, { f->fuzzify(); hasil = f->defuzzify(1); });
  selesai();
}
void loop() {}
