#include <iostream>
#include <immintrin.h>

extern "C" __m128 asm_mul_quat(float q1[4], float q2[4]);

extern "C" __m128 asm_permute(int a, int b,int c, int d, float in[4]);

int main()
{
    float q1[4] = {1,0,-1,0};
    float q2[4] = {1,-1,0,1};
    float q3[4] = {1,0,0,1};

    __m128 out = asm_permute(1,1,3,3, q3);

    float result[4];
    _mm_storeu_ps(result, out);
    for (int i = 0; i < 4; i++) {
        std::cout << result[i] << " ";
    }

    return 0;
}