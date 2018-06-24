#include "vm.h"

void v_syscall(v_registers* v_regs)
{
    __asm__
    (
        "movq %0, %%rax;"
        "movq %1, %%rdi;"
        "movq %2, %%rsi;"
        "movq %3, %%rdx;"
        "syscall;"
        :
        : "m" (v_regs->v_rax), "m" (v_regs->v_rdi), "m" (v_regs->v_rsi), "m" (v_regs->v_rdx)
        : "rax", "rdi", "rsi", "rdx"
    );
}
