#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/

make_instr_func(push_m)
{
    OPERAND m,esp;
    
    m.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &m);
    //设置esp的数据大小为32位，类型为寄存器（OPR_REG），地址设为0x4，对应于ESP寄存器 
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    //读取内存操作数和ESP寄存器的值
    operand_read(&m);
    operand_read(&esp);
    //将ESP的值减少4，这是因为推入一个32位的数据会使得堆栈顶部向低地址移动4个字节
    esp.val -= 4;
    //设置m的地址为更新后的esp.val，即新的堆栈顶部地址。
    //设置m的类型为内存操作数（OPR_MEM），段寄存器为堆栈段寄存器（SREG_SS）
    m.addr = esp.val;
    m.type = OPR_MEM;
    m.sreg = SREG_SS;
    //写入数据到堆栈和更新ESP
    operand_write(&esp);
    operand_write(&m);
    
    return len;
}

make_instr_func(push_short)
{
    OPERAND imm,esp;
    
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = 8;
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    //读取立即数和ESP寄存器
    operand_read(&imm);
    operand_read(&esp);
    //更新ESP寄存器
    esp.val -= 4;
    //准备写入堆栈
    imm.addr = esp.val;
    imm.type = OPR_MEM;
    imm.sreg = SREG_SS;
    //执行符号扩展，将imm.val与掩码0xFFFFFFFF >> (32 - data_size)进行按位与操作，保留低8位，然后通过sign_ext函数将其扩展至32位
    uint8_t k = 32 - data_size;
    imm.val = sign_ext(imm.val & (0xFFFFFFFF >> k), data_size);
    imm.data_size = 32;
    
    operand_write(&esp);
    operand_write(&imm);
    
    return 2;
}

make_instr_func(push_i_v)
{
    OPERAND imm,esp;
    
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = data_size;
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    //读取立即数和ESP寄存器的值
    operand_read(&imm);
    operand_read(&esp);
    
    esp.val -= 4;
    //准备写入堆栈
    imm.addr = esp.val;
    imm.type = OPR_MEM;
    imm.sreg = SREG_SS;

    uint8_t k = 32 - data_size;
    imm.val = sign_ext(imm.val & (0xFFFFFFFF >> k), data_size);
    imm.data_size = 32;
    //写入数据到堆栈和更新ESP
    operand_write(&esp);
    operand_write(&imm);
    
    return 1 + data_size / 8;
}

make_instr_func(push_r_v)
{
    OPERAND r,esp;

    r.data_size = 32;
    r.type = OPR_REG;
    r.addr = opcode & 0x7;
    //设置r的数据大小为32位，类型为寄存器，地址设为opcode & 0x7，这里opcode是操作码的一部分，& 0x7的操作是为了提取出低位的三位，用于确定要操作的寄存器
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;

    operand_read(&r);
    operand_read(&esp);

    esp.val -= 4;
    //将ESP的值减少4，这是因为推入一个32位的数据会使得堆栈顶部向低地址移动4个字节
    r.addr = esp.val;
    r.sreg = SREG_SS;
    r.type = OPR_MEM;

    operand_write(&esp);
    operand_write(&r);

    return 1;
}

make_instr_func(pusha){
    OPERAND r;
    r.data_size = 32;
    r.sreg = SREG_SS;
    r.type = OPR_MEM;
    //循环八次，每次循环：
    //减少ESP的值4个字节，为下一个32位寄存器留出空间。
    //设置r的值为当前寄存器的值（依次为EAX、ECX、EDX、EBX、原始ESP、EBP、ESI、EDI）。
    //设置r的地址为更新后的ESP值，即堆栈顶部的地址。
    //写入寄存器的值到堆栈顶部
    int k = 0;
    for(;k<8;k++)
    { 
    cpu.esp -= 4;
    r.val = cpu.eax;
    r.addr = cpu.esp;
    operand_write(&r);
    }
   
    return 1;
}
    
    
