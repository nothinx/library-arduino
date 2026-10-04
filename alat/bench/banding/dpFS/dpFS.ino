#include <FilterSensor.h>
RataRataBergerak<8> r; FilterMedian<5> m; FilterEMA e(FilterEMA::alphaDariWaktu(500, 20)); FilterKalman k(5, 1);
volatile float v;
void setup() { e.aturAlpha(FilterEMA::alphaDariWaktu(v, v)); k.aturNoise(v, v); }
void loop() { v = r.saring(v) + m.saring(v) + e.saring(v) + k.saring(v) + r.hasil() + m.hasil(); r.reset(); m.reset(); e.reset(); k.reset(); }
