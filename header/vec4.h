#ifndef TEST_ASM_VEC4_H
#define TEST_ASM_VEC4_H

#define EPSILON 0.0001f
#include <xmmintrin.h>

union alignas(16) vec4 {
    __m128 vector;
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
};

constexpr vec4 vec4_zero() {
    return vec4(_mm_setzero_ps());
}

constexpr vec4 vec4_one() {
    return vec4(_mm_set_ps1(1.0f));
}

inline vec4 __vectorcall operator+(const vec4 v1, const vec4 v2) {
    return vec4(_mm_add_ps(v1.vector, v2.vector));
}

inline vec4 __vectorcall operator-(const vec4 v1, const vec4 v2) {
    return vec4(_mm_sub_ps(v1.vector, v2.vector));
}

inline vec4 __vectorcall operator*(const vec4 v, const float f) {
    return vec4(_mm_mul_ps(v.vector , _mm_set_ps1(f)));
}

inline vec4 __vectorcall operator/(const vec4 v, const float f) {
    return vec4(_mm_div_ps(v.vector, _mm_set_ps1(f)));
}

inline vec4 __vectorcall operator-(const vec4 v) {
    return vec4(_mm_mul_ps(v.vector, _mm_set_ps1(-1.0f)));
}

inline float __vectorcall hadd(const vec4 v) {
    __m128 hi = _mm_movehl_ps(v.vector, v.vector);
    __m128 s  = _mm_add_ps(v.vector, hi);
    s = _mm_add_ss(s, _mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)));
    return _mm_cvtss_f32(s);
}

inline vec4 __vectorcall scale(const vec4 v1, const vec4 v2) {
    return vec4(_mm_mul_ps(v1.vector, v2.vector));
}

inline float __vectorcall dot_product(const vec4 v1, const vec4 v2) {
    return hadd(vec4(_mm_mul_ps(v1.vector, v2.vector)));
}

inline vec4 __vectorcall cross_product(const vec4 v1, const vec4 v2) {
    __m128 shufV1 = _mm_shuffle_ps(v1.vector, v1.vector, _MM_SHUFFLE(1, 2, 0, 0)); // Vec = (v1.y, v1.z, v1.x, ...)
    __m128 shufV2 = _mm_shuffle_ps(v2.vector, v2.vector, _MM_SHUFFLE(2, 0, 1, 0)); // Vec = (v2.z, v2.x, v2.y, ...)
    __m128 firstColumn = _mm_mul_ps(shufV1, shufV2);

    shufV1 = _mm_shuffle_ps(v1.vector, v1.vector, _MM_SHUFFLE(2, 0, 1, 0)); // Vec = (v1.z, v1.x, v1.y, ...)
    shufV2 = _mm_shuffle_ps(v2.vector, v2.vector, _MM_SHUFFLE(1, 2, 0, 0)); // Vec = (v2.y, v2.z, v2.x, ...)
    __m128 secondColumn = _mm_mul_ps(shufV1, shufV2);

    return vec4(_mm_sub_ps(firstColumn, secondColumn));
}

inline float __vectorcall length(const vec4 v) {
    __m128 sqrV = _mm_mul_ps(v.vector, v.vector);
    __m128 hi = _mm_movehl_ps(sqrV, sqrV);
    __m128 s  = _mm_add_ps(sqrV, hi);
    s = _mm_add_ss(s, _mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)));
    return _mm_cvtss_f32(_mm_sqrt_ss(s));
}
    
inline float __vectorcall sqrLength(const vec4 v) {
    __m128 sqrV = _mm_mul_ps(v.vector, v.vector);   
    return hadd(vec4(sqrV));
}

inline vec4 __vectorcall normalize(const vec4 v) {
    float length = length(v);
    return vec4(_mm_div_ps(v.vector, _mm_set_ps1(length)));
}

inline float __vectorcall distance(const vec4 v1, const vec4 v2) {
    return length(v1 - v2);
}

inline vec4 __vectorcall lerpUnclamped(const vec4 v1, const vec4 v2, float t) {
    return vec4((v2 - v1) * t + v1);
}

inline vec4 __vectorcall lerp(const vec4 v1, const vec4 v2, float t) {
    t = t < 0.0f ? 0.0f : t > 1.0f ? 1.0f : t;
    return lerpUnclamped(v1, v2, t);
}

inline bool __vectorcall operator==(const vec4 v1, const vec4 v2) {
    __m128 diff = _mm_sub_ps(v1.vector, v2.vector);
    
    // Get absolute value for each element
    __m128i mask_i = _mm_set1_epi32(0x7FFFFFFF); 
    __m128 mask = _mm_castsi128_ps(mask_i);
     diff = _mm_and_ps(diff, mask);
    
    // check if all element are less or equal than EPSILON
    __m128 result = _mm_cmple_ps(diff, _mm_set_ps1(EPSILON));
    return _mm_movemask_ps(result) == 0xFFFF;
}

#endif //TEST_ASM_VEC4_H
