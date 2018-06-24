#-*-coding:utf-8-*-

from capstone import *
import subprocess
import struct
import sys
import pdb

# INFORMATIONS
#
register_table = {
    "rax":    '\x00',
    "rbx":    '\x01',
    "rcx":    '\x02',
    "rdx":    '\x03',
    "rsi":    '\x04',
    "rdi":    '\x05',
    "rbp":    '\x06',
    "rsp":    '\x07',
    "r8" :    '\x08',
    "r9" :    '\x09',
    "r10":    '\x0a',
    "r11":    '\x0b',
    "r12":    '\x0c',
    "r13":    '\x0d',
    "r14":    '\x0e',
    "r15":    '\x0f',
    "rip":    '\x10',
    "rflags": '\x11',
    "tmp":    '\x12'
}
opcode_table = {
    "zero"     : '\xd0',

    "leaq"     : '\xd1',

    "movb"     : '\xd2',
    "movw"     : '\xd3',
    "movl"     : '\xd4',
    "movq"     : '\xd5',
    "movabsq"  : '\xd6',


    "addb"     : '\xd7',
    "addw"     : '\xd8',
    "addl"     : '\xd9',
    "addq"     : '\xda',

    "subb"     : '\xdb',
    "subw"     : '\xdc',
    "subl"     : '\xdd',
    "subq"     : '\xde',

    "syscall"  : '\xdf',

    "cmpb"     : '\xe0',
    "cmpw"     : '\xe1',
    "cmpl"     : '\xe2',
    "cmpq"     : '\xe3',

    "jmp"      : '\xe4',
    "je"       : '\xe5',
    "jne"      : '\xe6',

    "xorb"     : '\xe7',
    "xorw"     : '\xe8',
    "xorl"     : '\xe9',
    "xorq"     : '\xea'
    
}

class Code:
    def __init__(self, offset, opcode, operands):
        self.offset    = offset
        self.opcode    = opcode
        self.operands  = operands

        self.vcode = None

    def virtualization(self):
        #
        # Instruction Format
        #
        # AA BB CC DD
        #
        # AA: opcode
        # BC: operand info (a b c c a' b' c' c')
        #     a  : register bit
        #     b  : reference bit
        #     cc : size (00: 1, 01: 2, 10: 4, 11: 8)
        # DD: opernad 1 (Register: 1 byte, Immediate: size)
        # EE: operand 2 (Register: 1 byte, Immediate: size)
        
        # opcode
        #
        self.vcode = opcode_table[self.opcode]
        if len(self.operands) == 0:
            return self.vcode
        
        # operand info
        #
        v_info = 0b00000000

        # operand
        #
        v_operand = ""
        if self.opcode.startswith('j'):
            v_operand += '\x00'
        else:
            for i in range(len(self.operands)):
                operand = self.operands[i]
                this_info = 0b0000
                is_reg  = False
                is_ref  = False

                # REF
                if operand.startswith('(') and operand.endswith(')'):
                    is_ref = True
                    operand = operand.strip("()")
                # IMM
                if operand.startswith('$'):
                    if "-0x" in operand:
                        op_str = operand[4:]
                    elif "0x" in operand:
                        op_str = operand[3:]
                    else:
                        op_str = operand[1:]

                    operand = int(operand.strip('$'), 16)

                    if len(op_str) % 2 == 1:
                        op_str = '0' + op_str
                    size = len(op_str) / 2
                    if size == 1:
                        op_size = 0b00
                        if operand > 0x7f:
                            suffix = 'B'
                        else:
                            suffix = 'b'
                    elif size == 2:
                        op_size = 0b01
                        if operand > 0x7fff:
                            suffix = 'H'
                        else:
                            suffix = 'h'
                    elif size == 3 or size == 4:
                        op_size = 0b10
                        if operand > 0x7fffffff:
                            suffix = 'I'
                        else:
                            suffix = 'i'
                    elif 4 < size and size <= 8:
                        op_size = 0b11
                        if operand > 0x7fffffffffffffff:
                            suffix = 'Q'
                        else:
                            suffix = 'q'
                    else:
                        print"op_size[{}] error: IMM".format(i)
                        exit(-1)
                    try:
                        v_operand += struct.pack('<' + suffix, operand)
                    except struct.error:
                        if suffix == 'B':
                            op_size = 0b01
                            suffix = 'H'
                        elif suffix == 'b':
                            op_size = 0b01
                            suffix = 'h'
                        elif suffix == 'H':
                            op_size = 0b10
                            suffix = 'I'
                        elif suffix == 'h':
                            op_size = 0b10
                            suffix = 'i'
                        elif suffix == 'I':
                            op_size = 0b11
                            suffix = 'Q'
                        elif suffix == 'i':
                            op_size = 0b11
                            suffix = 'q'
                        v_operand += struct.pack('<' + suffix, operand)
                        
                # REG
                elif operand.startswith('%'):
                    is_reg = True
                    operand = operand.strip('%')
                    reg_name = operand[1]  # rax -> 'a', r8 -> '8'
                    if reg_name == 'm':
                        # tmp
                        #
                        op_size = 0b11
                    elif reg_name.isdigit():
                        # r8, r9, r10, r11, r12, r13, r14 r15
                        #
                        if operand.endswith('b'):
                            op_size = 0b00
                            operand = operand[:-1]
                        elif operand.endswith('w'):
                            op_size = 0b01
                            operand = operand[:-1]
                        elif operand.endswith('d'):
                            op_size = 0b10
                            operand = operand[:-1]
                        else:
                            op_size = 0b11
                    else:
                        # rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp
                        #
                        if operand.endswith('l'):
                            op_size = 0b00
                            operand = 'r' + operand[:-1]
                            if len(operand) == 2:
                                operand += 'x'

                        else:
                            if len(operand) == 2:
                                op_size = 0b01
                                operand = 'r' + operand
                            else:
                                if operand.startswith('r'):
                                    op_size = 0b11
                                elif operand.startswith('e'):
                                    op_size = 0b10
                                    operand = 'r' + operand[1:]
                                else:
                                    print "op_size[{}] error: REG".format(i)
                                    exit(-1)

                    v_operand += register_table[operand]  # append 1 byte

                this_info |= op_size
                if is_reg:
                    this_info |= 0b1000
                if is_ref:
                    this_info |= 0b0100
                if i==0:
                    this_info <<= 4
                v_info |= this_info
        
        self.vcode += chr(v_info) + v_operand

    def print_vcode(self):
        print '\t',
        for byte in self.vcode:
            print "{0:0>2}".format(hex(ord(byte))[2:]),
        print ''

    def print_assembly(self):
        print "{0:<8}{1:<8}".format(hex(self.offset).strip('L'), self.opcode),
        for operand in self.operands:
            print operand,
        print ""

def atnt_to_vsyntax(address, opcode, operands):
    vcode = []

    # this vm doesn't support indexed addressing
    #
    indexed = False
    for i in range(len(operands)):
        if '(' in operands[i]:
            if '+' in operands[i] or '-' in operands[i]:
                indexed = True
                break
    
    if indexed:
        offset, reg = operands[i].split('(')
        reg = reg.strip(')')
        operands[i] = "(%tmp)"

        vcode.append(Code(address, "movq", (reg, "%tmp")))  # mov REG, %tmp
        vcode.append(Code(0, "addq", ('$' + offset, "%tmp")))     # add OFFSET, %tmp
        vcode.append(Code(0, opcode, operands))            # OPCODE OPERAND1, (%tmp) 

    else:
        vcode.append(Code(address, opcode, operands))

    for i in range(len(operands)):
        if '$' not in operands[i] and "0x" in operands[i]:
            operands[i] = '$' + operands[i]

    return vcode
            

def divide_operand(operand_str):
    operands = []
    if operand_str != '':
        if ',' in operand_str:
            for operand in operand_str.split(','):
                operands.append(operand.strip(' '))
        else:
            operands.append(operand_str)

    return operands

def disassemble(filename, func_start, func_end):
    func_size = func_end - func_start + 1

    with open(filename, "rb") as f:
        f.seek(func_start)
        binary = f.read(func_size)

    codes = []
    md = Cs(CS_ARCH_X86, CS_MODE_64)
    md.syntax = CS_OPT_SYNTAX_ATT
    for inst in md.disasm(binary, func_start):
        address  = inst.address
        opcode   = inst.mnemonic
        operands = divide_operand(inst.op_str)

        codes += atnt_to_vsyntax(address, opcode, operands)

    return codes


def main(filename, func_start, func_end):
    codes = disassemble(filename, func_start, func_end)
    for code in codes:
        code.print_assembly()
        code.virtualization()
        code.print_vcode()
    # relocation branch offset
    #
    for i in range(len(codes)):
        if codes[i].opcode.startswith('j'):
            v_offset = 0

            origin_address = int(codes[i].operands[0].strip('$'), 16)
            if origin_address > codes[i].offset:
                direction = +1
            else:
                direction = -1

            j = i
            
            if direction == -1:
                v_offset += len(codes[j].vcode)

            while True:
                j += direction
                if codes[j].offset == origin_address:
                    if direction == -1:
                        v_offset += len(codes[j].vcode)
                    break
                else:
                    v_offset += len(codes[j].vcode)

            v_offset *= direction
            codes[i].operands[0] = v_offset

            if direction == -1:
                v_offset_str = hex(v_offset)[3:]
            else:
                v_offset_str = hex(v_offset)[2:]

            if len(v_offset_str) % 2 == 1:
                v_offset_str = '0' + v_offset_str

            v_offset_size = len(v_offset_str)/2
            if v_offset_size == 1:
                op_size = 0b000000
                suffix = 'b'
            elif v_offset_size == 2:
                op_size = 0b010000
                suffix = 'h'
            elif v_offset_size == 4:
                op_size = 0b100000
                suffix = 'l'
            elif v_offset_size == 8:
                op_size = 0b110000
                suffix = 'q'

            codes[i].vcode = codes[i].vcode[0] + chr(op_size) + struct.pack('<' + suffix, v_offset)
    # DEBUG LOG
    #
    for code in codes:
        code.print_vcode()

    # result
    #
    vcode = ""
    for code in codes:
        vcode += code.vcode
    vcode += '\x00'

    for i in range(len(vcode)):
        print "\'\\x{0:0>2}\',".format(hex(ord(vcode[i]))[2:]),


def get_func_address(filename, funcname):
    cmd = "objdump -d {}".format(filename)
    out = subprocess.check_output(cmd, shell=True)
    if "{}:\n".format(funcname) in out:
        out = out.split("{}:\n".format(funcname))[-1]
    elif "<{}>:\n".format(funcname) in out:
        out = out.split("<{}>:\n".format(funcname))[-1]
    out = out.split('\n')
    start_addr = int(out[0].split(':')[0], 16)

    for line in out:
        if "ret" in line:
            end_addr = int(line.split(':')[0], 16)
            break
    
    start_addr -= 0x400000
    end_addr   -= 0x400000

    return start_addr, end_addr

                

if __name__ == "__main__":

    # usage: virtualizer.py [filename] [funcname]
    #
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    else:
        filename = "./crackme/a.out"

    funcname = "func"  # virtualization target function's symbol name
    func_start, func_end = get_func_address(filename, funcname)

    # skip function's prologue and epilogue
    #
    func_start += 4  
    func_end   -= 2  # [WARN] this may not be accurate

    print "func_start: {}\n func_end: {}".format(hex(func_start), hex(func_end))

    main(filename, func_start, func_end)
