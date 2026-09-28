#include <iostream>
#include <chrono>
#include <xmmintrin.h>
#include "../header/vec4.h"

template<typename T, typename ...Args>
void test(T(*funct)(Args...), int count, Args... args)
{
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; i++)
        funct(args...);

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::cout << "Test was ongoing for " <<std::chrono::duration_cast<std::chrono::nanoseconds>(end - start) << std::endl;
}

void test(float(*funct)(vec4&, vec4&), int count, vec4& v1, vec4& v2)
{
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; i++)
        funct(v1, v2);

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::cout << "Test was ongoing for " <<std::chrono::duration_cast<std::chrono::nanoseconds>(end - start) << std::endl;
}

void test(float(*funct)(__m128&, __m128&), int count, __m128& v1, __m128& v2)
{
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; i++)
        funct(v1, v2);

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::cout << "Test was ongoing for " <<std::chrono::duration_cast<std::chrono::nanoseconds>(end - start) << std::endl;
}

extern "C" __m128 asm_mul_quat(float q1[4], float q2[4]);
extern "C" __m128 asm_permute(int a, int b,int c, int d, float in[4]);

float __declspec(noinline) test1(vec4& v1, vec4& v2)
{
    vec4 v3 = cross_product(v1, v2);
    return v3.x;
}

float __declspec(noinline) test2(__m128& m1, __m128& m2)
{
    __m128 shufV1 = _mm_shuffle_ps(m1, m1, _MM_SHUFFLE(1, 2, 0, 0)); // Vec = (v1.y, v1.z, v1.x, ...)
    __m128 shufV2 = _mm_shuffle_ps(m2, m2, _MM_SHUFFLE(2, 0, 1, 0)); // Vec = (v2.z, v2.x, v2.y, ...)
    __m128 firstColumn = _mm_mul_ps(shufV1, shufV2);

    shufV1 = _mm_shuffle_ps(m1, m1, _MM_SHUFFLE(2, 0, 1, 0)); // Vec = (v1.z, v1.x, v1.y, ...)
    shufV2 = _mm_shuffle_ps(m2, m2, _MM_SHUFFLE(1, 2, 0, 0)); // Vec = (v2.y, v2.z, v2.x, ...)
    __m128 secondColumn = _mm_mul_ps(shufV1, shufV2);

    __m128 m3 = _mm_sub_ps(firstColumn, secondColumn);
    return _mm_cvtss_f32(m3);
}

int main()
{
    vec4 v = {1,0,1,0};
    for (int i = 0; i < 100000; i++) {
        v = v * 3.0f;
    }
    
    vec4 v1 = {1,0,-1,0};
    vec4 v2 = {-1,1,0,0};
    test(test1, 1'000'000'000, v1, v2);
    
    __m128 m1 = _mm_setr_ps(1,0,-1,0);
    __m128 m2 = _mm_setr_ps(-1,1,0,0);
    test(test2, 1'000'000'000, m1, m2);

    return 0;
}