#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef struct v_registers
{
    long v_rax;
    long v_rbx;
    long v_rcx;
    long v_rdx;
    long v_rsi;
    long v_rdi;
    long v_rbp;
    long v_rsp;
    long v_r8;
    long v_r9;
    long v_r10;
    long v_r11;
    long v_r12;
    long v_r13;
    long v_r14;
    long v_r15;
    long v_rip;
    long v_rflags;
    long v_tmp;
}v_registers;

void v_zero(v_registers* v_regs);

void v_leaq(v_registers* v_regs);

void v_mov(v_registers* v_regs, int size);
void v_movb(v_registers* v_regs);
void v_movw(v_registers* v_regs);
void v_movl(v_registers* v_regs);
void v_movq(v_registers* v_regs);
void v_movabsq(v_registers* v_regs);

void v_add(v_registers* v_regs, int size);
void v_addb(v_registers* v_regs);
void v_addw(v_registers* v_regs);
void v_addl(v_registers* v_regs);
void v_addq(v_registers* v_regs);

void v_sub(v_registers* v_regs, int size);
void v_subb(v_registers* v_regs);
void v_subw(v_registers* v_regs);
void v_subl(v_registers* v_regs);
void v_subq(v_registers* v_regs);

void v_syscall(v_registers* v_regs);

void v_cmp(v_registers* v_regs, int size);
void v_cmpb(v_registers* v_regs);
void v_cmpw(v_registers* v_regs);
void v_cmpl(v_registers* v_regs);
void v_cmpq(v_registers* v_regs);

void v_jmp(v_registers* v_regs);
void v_je(v_registers* v_regs);
void v_jne(v_registers* v_regs);


void v_xor(v_registers* v_regs, int size);
void v_xorb(v_registers* v_regs);
void v_xorw(v_registers* v_regs);
void v_xorl(v_registers* v_regs);
void v_xorq(v_registers* v_regs);
/*
void v_rcr(v_registers* v_regs);
void v_rcrb(v_registers* v_regs);
void v_rcrw(v_registers* v_regs);
void v_rcrl(v_registers* v_regs);
void v_rcrq(v_registers* v_regs);

void v_rcl(v_registers* v_regs);
void v_rclb(v_registers* v_regs);
void v_rclw(v_registers* v_regs);
void v_rcll(v_registers* v_regs);
void v_rclq(v_registers* v_regs);
*/
