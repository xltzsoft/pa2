#include "cpu/instr.h"
/*
Put the implementations of `cmp' instructions here.
*/


make_instr_func(cmp_rm)
{
    OPERAND rm,imm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    //解析MOD R/M字节并确定内存操作数 
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = data_size;
    //读取rm和imm的值，从内存读取rm的值，从指令流读取imm的值
    operand_read(&rm);
    operand_read(&imm);
    
    alu_sub(imm.val, rm.val, data_size);
    return len + data_size / 8;
    //返回指令长度
}

make_instr_func(cmp_r_rm)
{
    OPERAND rm,r;
    
    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&r);
    operand_read(&rm);
    //与cmp_rm_r不同，这里的操作数顺序是rm.val减去r.val
    alu_sub(rm.val, r.val, data_size);
    return len;
}

make_instr_func(cmp_rm_r)
{
    OPERAND rm,r;
    
    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&r);
    operand_read(&rm);
    //调用alu_sub函数，将r.val和rm.val作为参数，以及data_size为数据大小
    //使用alu_sub函数计算imm.val - rm.val的结果，更新处理器的状态标志（例如ZF，SF，CF等），从而实现比较功能
    alu_sub(r.val, rm.val, data_size);
    return len;
}


make_instr_func(cmp_r_rm_short)
{
    OPERAND rm,r;
    
    rm.data_size = 8;
    r.data_size = 8;

    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&r);
    operand_read(&rm);
    //调用alu_sub函数，将rm.val和r.val作为参数，以及8作为数据大小参数。
    //alu_sub函数执行减法操作（实质上是进行比较），更新处理器的状态标志（如ZF、CF、SF等），用于后续条件跳转等操作。这里特别指出，操作数的位宽固定为8位
    alu_sub(rm.val, r.val, 8);
    return len;
}

make_instr_func(cmp_r_i_short)
{
    OPERAND r,imm;
    
    r.data_size = 8;
    r.addr = 0x0;
    r.type = OPR_REG;
    
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = 8;
    
    operand_read(&r);
    operand_read(&imm);
    //对imm.val和r.val进行掩码操作，仅保留低8位的值，以确保正确的位宽处理
    //对掩码后的imm.val和r.val进行符号扩展，以防止溢出并正确处理有符号数的比较
    //使用扩展后的值执行减法操作，实质上是比较两个值。更新处理器的状态标志（如ZF、CF、SF等），用于后续条件跳转等操作
    uint8_t k = 32 - data_size;
    alu_sub(sign_ext(imm.val & (0xFFFFFFFF >> k), data_size), sign_ext(r.val & (0xFFFFFFFF >> k), data_size), data_size);
    return 2;
}

make_instr_func(cmp_i2a_v){
    OPERAND r,imm;
    
    r.data_size = data_size;
    r.type = OPR_REG;
    r.addr = 0x0;
    
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = data_size;
    
    operand_read(&r);
    operand_read(&imm);
    alu_sub(imm.val, r.val, data_size);
    //调用alu_sub函数，将imm.val减去r.val，以及data_size作为数据大小参数。
    //alu_sub函数执行减法操作，实质上是比较两个值。更新处理器的状态标志（如ZF、CF、SF等），用于后续条件跳转等操作
    return 1 + data_size / 8;
} 


make_instr_func(cmp_rm_i_short)
{
    OPERAND rm,imm;
    
    rm.data_size = 8;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    //设置imm的操作数类型为立即数（OPR_IMM），地址为eip + len，即MOD R/M字节之后的第一个字节，数据大小设为8位
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = 8;
    
    operand_read(&rm);
    operand_read(&imm);
    
    uint8_t k = 32 - data_size;
    alu_sub(sign_ext(imm.val & (0xFFFFFFFF >> k), data_size), sign_ext(rm.val & (0xFFFFFFFF >> k), data_size), data_size);
    return len + 1;
}


make_instr_func(cmp_r2rm_b)
{
    OPERAND rm, r;
    
    rm.data_size = 8;
    r.data_size = 8;
    int len = 1;
    uint8_t k = 32 - data_size;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    //对r.val和rm.val进行掩码操作，仅保留低8位的值，以确保正确的位宽处理。
    //对掩码后的r.val和rm.val进行符号扩展，以防止溢出并正确处理有符号数的比较。
    //使用扩展后的值执行减法操作，实质上是比较两个值。更新处理器的状态标志（如ZF、CF、SF等），用于后续条件跳转等操作
    alu_sub(sign_ext(r.val & (0xFFFFFFFF >> k), data_size), sign_ext(rm.val & (0xFFFFFFFF >> k), data_size), data_size);
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(cmp_rm_i_v_short)
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

    if((imm.val & 0x80) != 0){
        imm.val = 0xFFFFFF00 | imm.val;
    }
    //如果imm.val的最高位（第8位）为1，说明立即数为负数
    //将imm.val与0xFFFFFF00进行按位或运算，确保高24位也被设置为1，从而进行正确的符号扩展
    alu_sub(imm.val, rm.val, data_size);
    return len + 1;
}
