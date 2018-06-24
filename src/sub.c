#include "vm.h"

void v_sub(v_registers* v_regs, int size)
{
    long rip = 0;
    unsigned char operand_info = 0;

    unsigned int op1_reg_bit  = 0;
    unsigned int op1_ref_bit  = 0;
    unsigned int op1_size_bit = 0;

    unsigned int op2_reg_bit  = 0;
    unsigned int op2_ref_bit  = 0;
    unsigned int op2_size_bit = 0;

    long reg_address = 0;

    long operand1 = 0;
    long operand2 = 0;

    rip = v_regs->v_rip;
    
    operand_info = *(char *)rip;
    rip += 1;

    op1_reg_bit  = operand_info & 0x80;  // 1 0 0 0 0 0 0 0
    op1_ref_bit  = operand_info & 0x40;  // 0 1 0 0 0 0 0 0
    op1_size_bit = operand_info & 0x30;  // 0 0 1 1 0 0 0 0
    op1_size_bit >>= 4;

    op2_reg_bit  = operand_info & 0x8;   // 0 0 0 0 1 0 0 0
    op2_ref_bit  = operand_info & 0x4;   // 0 0 0 0 0 1 0 0
    op2_size_bit = operand_info & 0x3;   // 0 0 0 0 0 0 1 1

    // REG
    //
    if (op1_reg_bit)
    {
        reg_address = (long *)v_regs + *(char *)rip;
        if (op1_size_bit == 0)
        {
            operand1 = *(char *)reg_address;   // al
        }
        else if (op1_size_bit == 1)
        {
            operand1 = *(short *)reg_address;  // ax
        }
        else if (op1_size_bit == 2)
        {
            operand1 = *(int *)reg_address;    // eax
        }
        else if (op1_size_bit == 3)
        {
            operand1 = *(long *)reg_address;   // rax
        }
        else
        {
            exit(-1);
        }
        rip += 1;
    }

    // IMM
    //
    else
    {
        if (op1_size_bit == 0)
        {
            operand1 = *(char *)rip;
            rip += 1;
        }
        else if (op1_size_bit == 1)
        {
            operand1 = *(short *)rip;
            rip += 2;
        }
        else if (op1_size_bit == 2)
        {
            operand1 = *(int *)rip;
            rip += 4;
        }
        else if (op1_size_bit == 3)
        {
            operand1 = *(long *)rip;
            rip += 8;
        }
        else
        {
            exit(-1);
        }

    }

    // REF
    //
    if (op1_ref_bit)
    {
        if (size == 1)
        {
            operand1 = *(char *)operand1;
        }
        else if (size == 2)
        {
            operand1 = *(short *)operand1;
        }
        else if (size == 4)
        {
            operand1 = *(int *)operand1;
        }
        else if (size == 8)
        {
            operand1 = *(long *)operand1;
        }
        else
        {
            exit(-1);
        }
    }

    // REG
    //
    if (op2_reg_bit)
    {
        reg_address = (long *)v_regs + *(unsigned char *)rip;
        operand2 = reg_address;
        rip += 1;
    }

    // IMM
    //
    else
    {
        if (op2_size_bit == 0)
        {
            operand2 = *(char *)rip;
            rip += 1;
        }
        else if (op2_size_bit == 1)
        {
            operand2 = *(short *)rip;
            rip += 2;
        }
        else if (op2_size_bit == 2)
        {
            operand2 = *(int *)rip;
            rip += 4;
        }
        else if (op2_size_bit == 3)
        {
            operand2 = *(long *)rip;
            rip += 8;
        }
        else
        {
            exit(-1);  // operand size error
        }
    }

    // REF
    //
    if (op2_ref_bit)
    {
        operand2 = *(long *)operand2;
    }

    // SUB
    //
    if (size == 1)
    {
        *(char *)operand2 -= operand1;
    }
    else if (size == 2)
    {
        *(short *)operand2 -= operand1;
    }
    else if (size == 4)
    {
        *(int *)operand2 -= operand1;
    }
    else if (size == 8)
    {
        *(long *)operand2 -= operand1;
    }
    else
    {
        exit(-1);
    }

    v_regs->v_rip = rip;
}

void v_subb(v_registers* v_regs)
{
    v_sub(v_regs, 1);
}

void v_subw(v_registers* v_regs)
{
    v_sub(v_regs, 2);
}

void v_subl(v_registers* v_regs)
{
    v_sub(v_regs, 4);
}

void v_subq(v_registers* v_regs)
{
    v_sub(v_regs, 8);
}
