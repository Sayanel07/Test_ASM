#include <iostream>
#include <chrono>
#include <xmmintrin.h>
#include "../header/vec4.h"

template<typename ...Args>
void test(void(*funct)(Args...), int count, Args... args)
{
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; i++)
        funct(args...);

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::cout << "Test was ongoing for " <<std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << std::endl;
}

extern "C" __m128 asm_mul_quat(float q1[4], float q2[4]);
extern "C" __m128 asm_permute(int a, int b,int c, int d, float in[4]);

int main()
{
    vec4 v1 = {1,0,-1,0};
    vec4 v2 = {-1,1,0,0};

    vec4 v3 = v1 + v2;

    std::cout << v3.z << std::endl;

    // float q1[4] = {1,0,-1,0};
    // float q2[4] = {1,-1,0,1};
    // float q3[4] = {1,0,0,1};
    //
    // __m128 out = asm_permute(1,1,3,3, q3);
    // float result[4];
    // _mm_storeu_ps(result, out);
    //
    // for (int i = 0; i < 4; i++) {
    //     std::cout << result[i] << " ";
    // }

    return 0;
}