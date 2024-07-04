#include "cpu/instr.h"

make_instr_func(jmp_near)
{
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);
    //计算符号扩展后的偏移量offset，确保正确处理负偏移量。
    //使用sign_ext函数，将相对偏移量扩展到适当的位宽，以便正确处理带符号的数值
    int offset = sign_ext(rel.val, data_size);
    //更新CPU的指令指针（cpu.eip），使其增加offset。实现了指令指针的跳转，使程序执行流转向新的位置
    cpu.eip += offset;

    return 1 + data_size / 8;
}


make_instr_func(jmp_near_indirect)
{
    OPERAND rm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    //如果data_size为16位，对rm.val进行掩码操作，只保留低16位的值，以确保目标地址的位宽正确
    if(data_size == 16){
        rm.val = rm.val & 0x0000FFFF;
    }
    //执行跳转
    cpu.eip = rm.val;

    return 0;
}

make_instr_func(jmp_far_imm){
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);
    
    cpu.eip = rel.val;
    
    rel.data_size = 16;
    rel.addr += data_size / 8;
    
    operand_read(&rel);
    //将读取到的代码段寄存器值赋给cpu.segReg[1].val，这里的1对应于代码段寄存器（CS）。
    //调用load_sreg(1)函数，涉及到加载新的段描述符到段寄存器，以确保新的代码段寄存器值被正确应用到处理器的段描述符表中
    cpu.segReg[1].val = rel.val;
    load_sreg(1);
    
    return 0;
}

make_instr_func(jmp_short)
{
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.data_size = 8;
    rel.addr = eip + 1;

    operand_read(&rel);

    if((rel.val & 0x80) != 0){
        rel.val = 0xFFFFFF00 | rel.val;
    }
    //处理相对偏移量的符号扩展
    //检查rel.val的最高位（第8位），如果该位为1，则表示这是一个负数（在二进制补码表示下）。这时，为了正确处理这个负数，需要将它扩展为32位的补码表示。
    //将rel.val的高24位（即从第9位到第32位）全部设置为1，这是通过与0xFFFFFF00进行按位或运算实现的
    int offset = sign_ext(rel.val, data_size);

    cpu.eip += offset;

    return 2;
}
