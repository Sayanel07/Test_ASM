//
// Created by aymer on 25/09/2026.
//

#ifndef TEST_ASM_VEC4_H
#define TEST_ASM_VEC4_H
#include <xmmintrin.h>


union vec4 {
    __m128 vector;
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
};

vec4 __vectorcall operator+(const vec4 &, const vec4 &);
vec4 __vectorcall operator-(const vec4 &, const vec4 &);
vec4 __vectorcall operator*(const vec4 &, const float &);
vec4 __vectorcall operator/(const vec4 &, const float &);
vec4 __vectorcall operator-(const vec4 &);

vec4 __vectorcall dot_product(const vec4&, const vec4&);
vec4 __vectorcall cross_product(const vec4&, const vec4&);


#endif //TEST_ASM_VEC4_H
