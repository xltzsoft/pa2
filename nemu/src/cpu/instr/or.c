#include "cpu/instr.h"
/*
Put the implementations of `or' instructions here.
*/

make_instr_func(or_rm_r)
{
    OPERAND rm, r;
    
    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    //调用alu_or函数，将r.val和rm.val作为参数，以及data_size作为数据大小参数。alu_or函数执行按位或操作，实质上是将rm.val和r.val的每一位进行或运算 
    rm.val = alu_or(r.val, rm.val, data_size);
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(or_r_rm_short)
{
    OPERAND rm, r;
    
    rm.data_size = 8;
    r.data_size = 8;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    //对rm.val和r.val进行掩码操作，仅保留低8位的值，以确保正确的位宽处理。
    //对掩码后的rm.val和r.val进行符号扩展，以防止溢出并正确处理有符号数的或运算
    uint8_t k = 32 - data_size; 
    r.val = alu_or(sign_ext(rm.val & (0xFFFFFFFF >> k), data_size), sign_ext(r.val & (0xFFFFFFFF >> k), data_size), data_size);
    
    operand_write(&r);
    
    return len;
}

make_instr_func(or_rm_i_v_short)
{
    OPERAND rm,imm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = 8;
    
    operand_read(&rm);
    operand_read(&imm);
    //处理立即数的符号扩展
    if((imm.val & 0x80) != 0){
        imm.val = 0xFFFFFF00 | imm.val;
    }
    //执行按位或操作
    rm.val = alu_or(imm.val, rm.val, data_size);
    //写回结果
    operand_write(&rm);
    
    return len + 1;
}

make_instr_func(or_i2rm_v)
{
    OPERAND rm,imm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    //初始化立即数操作数
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = data_size;
    //读取操作数
    operand_read(&rm);
    operand_read(&imm);
    rm.val = alu_or(imm.val, rm.val, data_size);
    operand_write(&rm);

    return len + data_size / 8;
}

make_instr_func(or_i2rm_b)
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
    
    if((rm.val & 0x80) != 0){
        rm.val = 0xFFFFFF00 | rm.val;
    }
    
    if((imm.val & 0x80) != 0){
        imm.val = 0xFFFFFF00 | imm.val;
    }
    
    rm.val = alu_or(imm.val, rm.val, data_size);
    operand_write(&rm);
    
    return len + 1;
}


make_instr_func(or_r2rm_b)
{
    OPERAND rm, r;
    
    rm.data_size = 8;
    r.data_size = 8;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    //对r.val和rm.val进行掩码操作，仅保留低8位的值，以确保正确的位宽处理。
    //对掩码后的r.val和rm.val进行符号扩展，以防止溢出并正确处理有符号数的或运算。
    uint8_t k = 32 - data_size;
    rm.val = alu_or(sign_ext(r.val & (0xFFFFFFFF >> k), data_size), sign_ext(rm.val & (0xFFFFFFFF >> k), data_size), data_size);
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(or_rm2r_v)
{
    OPERAND rm, r;
    
    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    //调用alu_or函数，将rm.val和r.val作为参数，以及data_size作为数据大小参数。alu_or函数执行按位或操作，实质上是将rm.val和r.val的每一位进行或运算
    r.val = alu_or(rm.val, r.val, data_size);
    
    operand_write(&r);
    
    return len;
}


make_instr_func(or_i2a_b)
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
    //对imm.val和r.val进行掩码操作，仅保留低8位的值，以确保正确的位宽处理。
    //对掩码后的imm.val和r.val进行符号扩展，以防止溢出并正确处理有符号数的或运算
    //将符号扩展后的imm.val和r.val作为参数，以及data_size作为数据大小参数。alu_or函数执行按位或操作，实质上是将imm.val和r.val的每一位进行或运算
    r.val = alu_or(sign_ext(imm.val & (0xFFFFFFFF >> k), data_size), sign_ext(r.val & (0xFFFFFFFF >> k), data_size), data_size);
    operand_write(&r);
    
    return 2;
}

make_instr_func(or_i2a_v)
{
    OPERAND r,imm;

    r.data_size = data_size;
    r.type = OPR_REG;
    r.addr = 0x0;

    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = data_size;

    operand_read(&r);
    operand_read(&imm);
    r.val = alu_or(imm.val, r.val, data_size);
    operand_write(&r);

    return 1 + data_size / 8;
}
