#include "vm.h"

void v_zero(v_registers* v_regs)
{
    int i=0;
    unsigned char *v_sp = v_regs->v_rbp;

    for (i=0; i<0x60; i++)
    {
        --v_sp;
        *v_sp = 0;
    }
}
