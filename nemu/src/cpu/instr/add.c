#include "cpu/instr.h"
/*
Put the implementations of `add' instructions here.
*/

make_instr_func(add_v)
{
    //初始化操作数和指令长度
    OPERAND rm,imm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = data_size;
    
    operand_read(&rm);
    operand_read(&imm);
    rm.val = alu_add(imm.val, rm.val, data_size);
    //调用alu_add函数，将imm.val和rm.val作为参数，以及data_size作为数据大小参数。alu_add函数执行加法操作并返回结果
    operand_write(&rm);
    
    return len + data_size / 8;
    //函数最后返回整个指令的长度（以字节为单位）
    //len代表MOD R/M字节和其他可能的指令字节，data_size / 8转换data_size从位到字节，以计算立即数操作数占据的空间。
}

make_instr_func(add_rm_i_v_short)
{
   //初始化操作数和指令长度
    OPERAND rm,imm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = 8;
    
    operand_read(&rm);
    operand_read(&imm);
    
    if((imm.val & 0x80) != 0){
        imm.val = 0xFFFFFF00 | imm.val;
    }
    //如果imm.val的最高位（第8位）为1，说明立即数为负数。此时，通过将imm.val与0xFFFFFF00进行按位或运算，确保高24位也被设置为1，从而进行正确的符号扩展。
    rm.val = alu_add(imm.val, rm.val, data_size);
    
    operand_write(&rm);
    
    return len + 1;
}

make_instr_func(add_r_i)
{
    OPERAND r,imm;
    
    r.data_size = data_size;
    r.type = OPR_REG;
    r.addr = 0x0;
    //设置r的操作数类型为寄存器，地址设为0x0，设置imm的操作数类型为立即数，地址为eip + 1，即当前指令指针后的第一个字节，数据大小设为data_size
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = data_size;
    
    operand_read(&r);
    operand_read(&imm);
    r.val = alu_add(imm.val, r.val, data_size);
    operand_write(&r);
    
    return 1 + data_size / 8;
    //函数最后返整个指令的长度（以字节为单位）
    //1代表指令的前缀或MOD R/M字节，data_size / 8转换data_size从位到字节，以计算立即数操作数占据的空间。
}

make_instr_func(add_rm_r_short)
{
    OPERAND rm, r;
    
    rm.data_size = 8;
    r.data_size = 8;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    //执行加法,符号扩展
    rm.val = alu_add(sign_ext(r.val & (0xFFFFFFFF >> (32 - data_size)), data_size), sign_ext(rm.val & (0xFFFFFFFF >> (32 - data_size)), data_size), data_size);
    
    operand_write(&rm);
    //将计算得到的新值赋给rm.val，结果写回到原来从内存读取rm值的位置
    return len;
    //函数最后返整个指令的长度（以字节为单位）
}

make_instr_func(add_rm_r)
{
    OPERAND rm, r;
    
    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    //调用modrm_rm函数，传入eip + 1（当前指令指针后的第一个字节）以及指向r和rm的指针。
    //解析MOD R/M字节，确定内存操作数的地址和类型，并返回所解析字节的长度。len变量加上返回的长度，确保len包含了MOD R/M字节的长度
    operand_read(&rm);
    operand_read(&r);
    
    rm.val = alu_add(r.val, rm.val, data_size);
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(add_i2rm_b)
{
    OPERAND rm,imm;
    
    rm.data_size = 8;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = 8;
    
    operand_read(&rm);
    operand_read(&imm);
    
   //检查imm.val的最高位（第8位），如果为1，说明立即数为负数。
   //通过将imm.val与0xFFFFFF00进行按位或运算，确保高24位也被设置为1，进行正确的符号扩展。同样，检查rm.val的最高位（第8位），如果为1，也进行相同的符号扩展处理 
    if((rm.val & 0x80) != 0){
        rm.val = 0xFFFFFF00 | rm.val;
    }
    if((imm.val & 0x80) != 0){
        imm.val = 0xFFFFFF00 | imm.val;
    }

    //将计算得到的新值赋给rm.val，结果写回到原来从内存读取rm值的位置
    rm.val = alu_add(imm.val, rm.val, data_size);
    operand_write(&rm);
    
    return len + 1;
}

make_instr_func(add_rm2r_b)
{
    OPERAND rm, r;
    
    rm.data_size = 8;
    r.data_size = 8;
    int len = 1;
    uint8_t k = 32 - data_size;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    //执行加法操作前的数据准备
    r.val = alu_add(sign_ext(rm.val & (0xFFFFFFFF >> k), data_size), sign_ext(r.val & (0xFFFFFFFF >> k), data_size), data_size);
    
    operand_write(&r);
    
    return len;
}

make_instr_func(add_i2a_b)
{
    OPERAND r,imm;
    
    r.data_size = 8;
    r.type = OPR_REG;
    r.addr = 0x0;
    
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = 8;
    uint8_t k = 32 - data_size;

    operand_read(&r);
    operand_read(&imm);
    //执行加法操作前的数据准备
    r.val = alu_add(sign_ext(imm.val & (0xFFFFFFFF >> k), data_size), sign_ext(r.val & (0xFFFFFFFF >> k), data_size), data_size);
    operand_write(&r);
    
    return 2;
}

make_instr_func(add_rm2r_v)
{
    OPERAND rm, r;

    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);

    operand_read(&rm);
    operand_read(&r);

    r.val = alu_add(rm.val, r.val, data_size);
    //将计算得到的新值赋给r.val，结果写回到寄存器r中
    operand_write(&r);

    return len;
}

