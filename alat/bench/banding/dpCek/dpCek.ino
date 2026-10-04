#include <math.h>
volatile float v;
void setup(){ v = v * 0.5; v = exp(-v / v); }
void loop(){}
