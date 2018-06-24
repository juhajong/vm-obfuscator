#include "vm.h"

/* Instruction Format
 *
 * AA BC DD EE
 *
 * AA: opcode
 * BC: operand info ( 0 0 0 0 a b a b)
 *      a: reference bit
 *      b: register bit
 * DD: operand1 (Register: 1 byte, Immediate: 8 bytes)
 * EE: operand2 (Register: 1 byte, Immediate: 8 bytes)
 */

/* Stack Layout
 *               __________
 *              [          ]
 *              [  ret     ]
 *             _[  sfp     ]_rbp
 *  VM Data <-| [ v_tmp    ]_rbp-0x08
 *            | [ v_rflags ]_rbp-0x10
 *            | [ v_rip    ]_rbp-0x18
 *            | [ v_r15    ]_rbp-0x20
 *            | [ v_r14    ]_rbp-0x28
 *            | [ v_r13    ]_rbp-0x30
 *            | [ v_r12    ]_rbp-0x38
 *            | [ v_r11    ]_rbp-0x40
 *            | [ v_r10    ]_rbp-0x48
 *            | [ v_r9     ]_rbp-0x50
 *            | [ v_r8     ]_rbp-0x58
 *            | [ v_rsp    ]_rbp-0x60
 *            | [ v_rbp    ]_rbp-0x68
 *            | [ v_rdi    ]_rbp-0x70
 *            | [ v_rsi    ]_rbp-0x78
 *            | [ v_rdx    ]_rbp-0x80
 *            | [ v_rcx    ]_rbp-0x88
 *            | [ v_rbx    ]_rbp-0x90
 *            |_[ v_rax    ]_rbp-0x98, v_rbp, v_rsp
 *              [ vstack   ]
 *              [ (0x60)   ]_rbp-0xf8
 *              [__________]
 */

void (*v_table[])(v_registers *v_regs) = 
{
    v_zero,

    v_leaq,

    v_movb,
    v_movw,
    v_movl,
    v_movq,
    v_movabsq,

    v_addb,
    v_addw,
    v_addl,
    v_addq,

    v_subb,
    v_subw,
    v_subl,
    v_subq,

    v_syscall,

    v_cmpb,
    v_cmpw,
    v_cmpl,
    v_cmpq,

    v_jmp,
    v_je,
    v_jne,

    v_xorb,
    v_xorw,
    v_xorl,
    v_xorq
/*
    v_rcrb,
    v_rcrw,
    v_rcrl,
    v_rcrq,

    v_rclb,
    v_rclw,
    v_rcll,
    v_rclq
    */
};

char v_code[] = {'\xd0', '\xd5', '\xbb', '\x06', '\x12', '\xda', '\x0b', '\xec', '\x12', '\xd4', '\xaf', '\x05', '\x12', '\xd5', '\xbb', '\x06', '\x12', '\xda', '\x0b', '\xe8', '\x12', '\xd4', '\xaf', '\x04', '\x12', '\xd5', '\xbb', '\x06', '\x12', '\xda', '\x0b', '\xec', '\x12', '\xd4', '\xfa', '\x12', '\x00', '\xd5', '\xbb', '\x06', '\x12', '\xda', '\x0b', '\xfc', '\x12', '\xd4', '\xaf', '\x00', '\x12', '\xd5', '\xbb', '\x06', '\x12', '\xda', '\x0b', '\xe8', '\x12', '\xd4', '\xfa', '\x12', '\x00', '\xd5', '\xbb', '\x06', '\x12', '\xda', '\x0b', '\xf8', '\x12', '\xd4', '\xaf', '\x00', '\x12', '\xd5', '\xbb', '\x06', '\x12', '\xda', '\x0b', '\xfc', '\x12', '\xd4', '\xfa', '\x12', '\x00', '\xd5', '\xbb', '\x06', '\x12', '\xda', '\x0b', '\xf8', '\x12', '\xe9', '\xfa', '\x12', '\x00', '\x00'};

int volatile func(int x, int y)
{
    __asm__
    (
        "construct:;"
        "   sub $0x150, %%rsp;"
        "   leaq -0x98(%%rbp), %%rax;"
        "   movq %%rax, -0x68(%%rbp);"  // Set v_rbp
        "   movq %%rax, -0x60(%%rbp);"  // Set v_rsp
        "   leaq %0, %%rax;"
        "   movq %%rax, -0x18(%%rbp);"  // Set v_rip
        "   movq %2, %%rax;"
        "   movq %%rax, -0x70(%%rbp);"  // set v_rdi (argv[0])
        "   movq %3, %%rax;"
        "   movq %%rax, -0x78(%%rbp);"  // set v_rsi (argv[1])
        "fetch:;"
        "   xor %%rax, %%rax;"
        "   movq -0x18(%%rbp), %%rbx;"
        "   movb (%%rbx), %%al;"
        "   incq -0x18(%%rbp);"
        "   test %%al, %%al;"
        "   je destruct;"
        "execute:;"
        "   leaq -0x98(%%rbp), %%rdi;"
        "   sub $0xd0, %%rax;"
        "   imul $8, %%rax;"
        "   leaq %1, %%rbx;"
        "   addq %%rax, %%rbx;"
        "   call *(%%rbx);"
        "   jmp fetch;"
        "destruct:;"
        "   movq -0x98(%%rbp), %%rax;"
        "   mov %%rbp, %%rsp;"
        "   pop %%rbp;"
        "   retn;"
        :
        : "m" (v_code), "m" (v_table), "m" (x), "m" (y)
        :
    );
}

int main(void)
{
    int x, y;
    printf("x: ");
    scanf("%d", &x);
    printf("y: ");
    scanf("%d", &y);
    printf("result: %d\n", func(x, y));
    return 0;
}
