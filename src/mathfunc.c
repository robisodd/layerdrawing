#include <pebble.h>
#include "mathfunc.h"
//#define MATH_PI 3.141592653589793238462
//float sine_rad(float theta) {return (float)sin_lookup(TRIG_MAX_ANGLE * theta / (MATH_PI * 2)) / (float)TRIG_MAX_RATIO;} //sin(radians=0-2pi)
//float cosine_rad(float theta) {return (float)cos_lookup(TRIG_MAX_ANGLE * theta / (MATH_PI * 2)) / (float)TRIG_MAX_RATIO;} //cos(radians=0-2pi)

//float floor_float(float a){int32_t b=(int32_t)a; if((float)b!=a) if(a<0)b--; return (float)b;}
//float  ceil_float(float a){int32_t b=(int32_t)a; if((float)b!=a) if(a>0)b++; return (float)b;}

#define root_depth 20          // How many iterations square root function performs
int32_t  sqrt32(int32_t a) {int32_t b=a; for(int8_t i=0; i<root_depth; i++) b=(b+(a/b))/2; return b;} // Square Root

int32_t abs32(int32_t x) {return (x^(x>>31)) - (x>>31);}
int16_t abs16(int16_t x) {return (x^(x>>15)) - (x>>15);}
int8_t  abs8 (int8_t  x) {return (x^(x>> 7)) - (x>> 7);}

int8_t  sign8 (int8_t  x){return (x > 0) - (x < 0);}
int16_t sign16(int16_t x){return (x > 0) - (x < 0);}
int32_t sign32(int32_t x){return (x > 0) - (x < 0);}

