#include "vm.h"

void v_jmp(v_registers* v_regs)
{
    long rip = 0;
    unsigned char operand_info = 0;

    unsigned int op1_reg_bit  = 0;
    unsigned int op1_ref_bit  = 0;
    unsigned int op1_size_bit = 0;

    long reg_address = 0;

    long operand1 = 0;

    rip = v_regs->v_rip;
    
    operand_info = *(char *)rip;
    rip += 1;

    op1_reg_bit  = operand_info & 0x80;  // 1 0 0 0 0 0 0 0
    op1_ref_bit  = operand_info & 0x40;  // 0 1 0 0 0 0 0 0
    op1_size_bit = operand_info & 0x30;  // 0 0 1 1 0 0 0 0
    op1_size_bit >>= 4;

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
        if (op1_size_bit == 0)
        {
            operand1 = *(char *)operand1;
        }
        else if (op1_size_bit == 1)
        {
            operand1 = *(short *)operand1;
        }
        else if (op1_size_bit == 2)
        {
            operand1 = *(int *)operand1;
        }
        else if (op1_size_bit == 3)
        {
            operand1 = *(long *)operand1;
        }
        else
        {
            exit(-1);
        }
    }
    
    // JMP
    //
    rip += operand1;

    v_regs->v_rip = rip;
}


void v_je(v_registers* v_regs)
{
    long rip = 0;
    unsigned char operand_info = 0;
    unsigned int op1_reg_bit  = 0;
    unsigned int op1_size_bit = 0;
    unsigned char zf = 0;
    
    zf = v_regs->v_rflags & 0b1000000;

    if (zf != 0)
    {
        v_jmp(v_regs);
    }

    // fetch operands to skip rip
    //
    else
    {
        rip = v_regs->v_rip;
        operand_info = *(char *)rip;
        rip += 1;

        op1_reg_bit  = operand_info & 0x80;  // 1 0 0 0 0 0 0 0
        op1_size_bit = operand_info & 0x30;  // 0 0 1 1 0 0 0 0
        op1_size_bit >>= 4;

        // REG
        //
        if (op1_reg_bit)
        {
            rip += 1;
        }

        // IMM
        //
        else
        {
            if (op1_size_bit == 0)
            {
                rip += 1;
            }
            else if (op1_size_bit == 1)
            {
                rip += 2;
            }
            else if (op1_size_bit == 2)
            {
                rip += 4;
            }
            else if (op1_size_bit == 3)
            {
                rip += 8;
            }
            else
            {
                exit(-1);
            }

        }

        v_regs->v_rip = rip;
    }
}


void v_jne(v_registers* v_regs)
{
    long rip = 0;
    unsigned char operand_info = 0;
    unsigned int op1_reg_bit  = 0;
    unsigned int op1_size_bit = 0;
    unsigned char zf = 0;
    
    zf = v_regs->v_rflags & 0b1000000;

    if (zf == 0)
    {
        v_jmp(v_regs);
    }

    // fetch operands to skip rip
    //
    else
    {
        rip = v_regs->v_rip;
        operand_info = *(char *)rip;
        rip += 1;

        op1_reg_bit  = operand_info & 0x80;  // 1 0 0 0 0 0 0 0
        op1_size_bit = operand_info & 0x30;  // 0 0 1 1 0 0 0 0
        op1_size_bit >>= 4;

        // REG
        //
        if (op1_reg_bit)
        {
            rip += 1;
        }

        // IMM
        //
        else
        {
            if (op1_size_bit == 0)
            {
                rip += 1;
            }
            else if (op1_size_bit == 1)
            {
                rip += 2;
            }
            else if (op1_size_bit == 2)
            {
                rip += 4;
            }
            else if (op1_size_bit == 3)
            {
                rip += 8;
            }
            else
            {
                exit(-1);
            }

        }

        v_regs->v_rip = rip;
    }
}
