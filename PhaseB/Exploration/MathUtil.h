#ifndef MATH_UTIL_H
#define MATH_UTIL_H

// Some conversions
#define MS_TO_S(ms) ((ms) / 1000.0)
#define S_TO_MS(s) ((s) * 1000.0)
#define RAD2DEG(val) ((val)*180.0/PI)
#define DEG2RAD(val) ((val)*PI/180.0)


#define CLAMP(min_val, val, max_val) min(max((min_val), (val)), (max_val))

#define PI 3.1415

// inline static float normaliseTheta(float theta) {
//    double r = fmod(theta - PI, 2.0*PI);
//    return r > 0 ? (r - PI) : (r + PI);
// }

#endif // MATH_UTIL_H