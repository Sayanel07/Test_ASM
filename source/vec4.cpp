//
// Created by aymer on 25/09/2026.
//

#include "../header/vec4.h"

vec4 __vectorcall operator+(const vec4& v1, const vec4& v2) {
    return vec4{_mm_add_ps(v1.vector, v2.vector)};
}

vec4 __vectorcall operator-(const vec4& v1, const vec4& v2) {
    return vec4{_mm_sub_ps(v1.vector, v2.vector)};
}

vec4 __vectorcall operator*(const vec4& v, const float& x) {
    return vec4{_mm_mul_ps(v.vector, _mm_set_ps1(x))};
}

vec4 __vectorcall operator/(const vec4& v, const float& x) {
    return vec4{_mm_div_ps(v.vector, _mm_set_ps1(x))};
}

vec4 __vectorcall operator-(const vec4& v) {
    return vec4{_mm_mul_ps(v.vector, _mm_set_ps1(-1.f))};
}

vec4 __vectorcall dot_product(const vec4& v1, const vec4& v2) {
    return vec4{_mm_mul_ps(v1.vector, v2.vector)};
}

vec4 __vectorcall cross_product(const vec4& v1, const vec4& v2) {
    __m128 shufV1 = _mm_shuffle_ps(v1.vector, v1.vector, _MM_SHUFFLE(1, 2, 0, 0)); // Vec = (v1.y, v1.z, v1.x)
    __m128 shufV2 = _mm_shuffle_ps(v2.vector, v2.vector, _MM_SHUFFLE(2, 0, 1, 0)); // Vec = (v2.z, v2.x, v2.y)
    __m128 firstColumn = _mm_mul_ps(shufV1, shufV2);

    shufV1 = _mm_shuffle_ps(v1.vector, v1.vector, _MM_SHUFFLE(2, 0, 1, 0));
    shufV2 = _mm_shuffle_ps(v2.vector, v2.vector, _MM_SHUFFLE(1, 2, 0, 0));
    __m128 secondColumn = _mm_mul_ps(shufV1, shufV2);

    return vec4{_mm_sub_ps(firstColumn, secondColumn)};
}
