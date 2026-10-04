// Pembanding FilterSensor vs pesaing (scratchpad, bukan bagian repo).
#include "Siklus.h"
#include <FilterSensor.h>
#include <RunningAverage.h>
#include <RunningMedian.h>
#include <movingAvg.h>
#include <SimpleKalmanFilter.h>
#include <Ewma.h>

static float data[64];
static int dataInt[64];
volatile float out;
volatile int outInt;

RataRataBergerak<8> rata8;
FilterMedian<5> med5;
FilterMedian<15> med15;
FilterEMA ema(0.2f);
FilterKalman kal(5, 1);
RunningAverage ra8(8);
RunningMedian rm5(5), rm15(15);
movingAvg ma8(8);
SimpleKalmanFilter skf(5, 5, 0.01f);
Ewma ew(0.2);

#define CETAK(nama, nilai) do { Serial.print(F("BENCH " nama " ")); Serial.println((unsigned)(nilai)); } while (0)

void setup() {
  Serial.begin(115200);
  uint32_t a = 12345;
  for (int i = 0; i < 64; i++) {
    a = a * 1664525UL + 1013904223UL;
    data[i] = 500 + (int)((a >> 20) % 41) - 20 + ((a >> 8) % 20 == 0 ? 300 : 0);
    dataInt[i] = (int)data[i];
  }
  ma8.begin();
  for (int i = 0; i < 64; i++) { // isi penuh dulu
    rata8.saring(data[i]); med5.saring(data[i]); med15.saring(data[i]); ema.saring(data[i]); kal.saring(data[i]);
    ra8.add(data[i]); rm5.add(data[i]); rm15.add(data[i]); ma8.reading(dataInt[i]); skf.updateEstimate(data[i]); ew.filter(data[i]);
  }
  UKUR("kosong", 1000, out = data[_i & 63]);
  UKUR("kita_rata8", 1000, out = rata8.saring(data[_i & 63]));
  UKUR("RA_rata8_fast", 1000, { ra8.add(data[_i & 63]); out = ra8.getFastAverage(); });
  UKUR("RA_rata8_getAverage", 1000, { ra8.add(data[_i & 63]); out = ra8.getAverage(); });
  UKUR("movingAvg_int8", 1000, outInt = ma8.reading(dataInt[_i & 63]));
  UKUR("kita_med5", 1000, out = med5.saring(data[_i & 63]));
  UKUR("RM_med5", 1000, { rm5.add(data[_i & 63]); out = rm5.getMedian(); });
  UKUR("kita_med15", 1000, out = med15.saring(data[_i & 63]));
  UKUR("RM_med15", 1000, { rm15.add(data[_i & 63]); out = rm15.getMedian(); });
  UKUR("kita_ema", 1000, out = ema.saring(data[_i & 63]));
  UKUR("Ewma_ema", 1000, out = ew.filter(data[_i & 63]));
  UKUR("kita_kalman", 1000, out = kal.saring(data[_i & 63]));
  UKUR("SKF_kalman", 1000, out = skf.updateEstimate(data[_i & 63]));
  UKUR("kita_alphaDariWaktu", 100, out = FilterEMA::alphaDariWaktu(500, data[_i & 63] * 0.01f));
  CETAK("sizeof_rata8", sizeof(rata8)); CETAK("sizeof_med5", sizeof(med5)); CETAK("sizeof_med15", sizeof(med15));
  CETAK("sizeof_ema", sizeof(ema)); CETAK("sizeof_kalman", sizeof(kal));
  CETAK("sizeof_RA", sizeof(ra8)); CETAK("sizeof_RM", sizeof(rm5)); CETAK("sizeof_movingAvg", sizeof(ma8));
  CETAK("sizeof_SKF", sizeof(skf)); CETAK("sizeof_Ewma", sizeof(ew));
  selesai();
}
void loop() {}
