option casemap:none     ; noms sensibles à la casse (comme en C++)

.data                   ; variables globales initialisées

.const                  ; données en lecture seule
wzyx REAL4 1.0, -1.0, 1.0, -1.0
zwxy REAL4 1.0, 1.0, -1.0, -1.0
yxwz REAL4 -1.0, 1.0, 1.0, -1.0

.code                   ; code
asm_add PROC
    lea     rax, [rcx + rdx]
    ret
asm_add ENDP

asm_permute PROC
    sub rsp, 16

    shl ecx, 2 ; equivalent to x4
    shl edx, 2
    shl r8d, 2
    shl r9d, 2

    mov [rsp + 0], cl ; take the first bytes of rcx and add it to the stack
    lea eax, [ecx + 1] ; put the second bytes of rcx at the start of rax
    mov [rsp + 1], al ; take the first bit of rax and add it to the stack
    lea eax, [ecx + 2]
    mov [rsp + 2], al
    lea eax, [ecx + 3]
    mov [rsp + 3], al

    mov [rsp + 4], dl
    lea eax, [edx + 1]
    mov [rsp + 5], al
    lea eax, [edx + 2]
    mov [rsp + 6], al
    lea eax, [edx + 3]
    mov [rsp + 7], al

    mov [rsp + 8], r8b
    lea eax, [r8d + 1]
    mov [rsp + 9], al
    lea eax, [r8d + 2]
    mov [rsp + 10], al
    lea eax, [r8d + 3]
    mov [rsp + 11], al

    mov [rsp + 12], r9b
    lea eax, [r9d + 1]
    mov [rsp + 13], al
    lea eax, [r9d + 2]
    mov [rsp + 14], al
    lea eax, [r9d + 3]
    mov [rsp + 15], al

    movdqa xmm1, [rsp]

    pshufb xmm0, xmm1

    add rsp, 16
    ret
asm_permute ENDP

asm_mul_quat PROC
    sub rsp, 16
    movups [rsp], xmm6
    sub rsp, 16
    movups [rsp], xmm7
    sub rsp, 16
    movups [rsp], xmm8

    movaps  xmm0, XMMWORD PTR [rcx] ; q1
    movaps  xmm1, XMMWORD PTR [rdx] ; q2
    movaps  xmm2, XMMWORD PTR [wzyx] ; wzyx
    movaps  xmm3, XMMWORD PTR [zwxy] ; zwxy
    movaps  xmm4, XMMWORD PTR [yxwz] ; yxwz

    movaps  xmm5, xmm1 ; q2x
    movaps  xmm6, xmm1 ; q2y
    movaps  xmm7, xmm1 ; q2z
    movaps  xmm8, xmm1 ; result

    shufps xmm5, xmm5, 00h
    shufps xmm6, xmm6, 55h
    shufps xmm7, xmm7, 0AAh
    shufps xmm8, xmm8, 0FFh

    mulps xmm8, xmm0

    ;shuffle q1 to make multiplication easier
    shufps xmm0, xmm0, 00011011b
    mulps xmm5, xmm0
    mulps xmm5, xmm2
    addps xmm8, xmm5

    shufps xmm0, xmm0, 01001110b
    mulps xmm6, xmm0
    mulps xmm6, xmm3

    shufps xmm0, xmm0, 00011011b
    mulps xmm7, xmm0
    mulps xmm7, xmm4

    addps xmm6, xmm7
    addps xmm8, xmm6

    movaps xmm0, xmm8

    movups xmm8, [rsp]
    add rsp, 16
    movups xmm7, [rsp]
    add rsp, 16
    movups xmm6, [rsp]
    add rsp, 16
done:
    ret
asm_mul_quat ENDP

END