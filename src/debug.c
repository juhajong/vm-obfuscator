#include "vm.h"

void v_debug(v_registers* v_regs)
{
    int i=0, j=0;
    unsigned char *v_sp = v_regs->v_rbp;
    v_sp -= 0x60;

    printf("\n\nREGISTER SET\n");
    printf("\tv_rax   : 0x%lx\n", v_regs->v_rax);
    printf("\tv_rbx   : 0x%lx\n", v_regs->v_rbx);
    printf("\tv_rcx   : 0x%lx\n", v_regs->v_rcx);
    printf("\tv_rdx   : 0x%lx\n", v_regs->v_rdx);
    printf("\tv_rsi   : 0x%lx\n", v_regs->v_rsi);
    printf("\tv_rdi   : 0x%lx\n", v_regs->v_rdi);
    printf("\tv_rbp   : 0x%lx\n", v_regs->v_rbp);
    printf("\tv_rsp   : 0x%lx\n", v_regs->v_rsp);
    printf("\tv_r8    : 0x%lx\n", v_regs->v_r8 );
    printf("\tv_r9    : 0x%lx\n", v_regs->v_r9 );
    printf("\tv_r10   : 0x%lx\n", v_regs->v_r10);
    printf("\tv_r11   : 0x%lx\n", v_regs->v_r11);
    printf("\tv_r12   : 0x%lx\n", v_regs->v_r12);
    printf("\tv_r13   : 0x%lx\n", v_regs->v_r13);
    printf("\tv_r14   : 0x%lx\n", v_regs->v_r14);
    printf("\tv_r15   : 0x%lx\n", v_regs->v_r15);
    printf("\tv_rip   : 0x%lx\n", v_regs->v_rip);
    printf("\tv_rflags: 0x%lx\n", v_regs->v_rflags);
    printf("\tv_tmp   : 0x%lx\n\n", v_regs->v_tmp);
    
    printf("STACK LATOUT    --------------------------------------------------\n");
    for (i=6; i>0; --i)
    {
        printf("0x%lx [ ", v_sp);
        for (j=0; j<0x10; ++j)
        {
            if (j==8)
            {
                printf(" ");
            }
            printf("%02x ", *v_sp);
            ++v_sp;
        }
        printf("]\n");
    }
    printf("                --------------------------------------------------\n");

}
