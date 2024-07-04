#include "cpu/instr.h"
/*
Put the implementations of `and' instructions here.
*/

make_instr_func(and_rm_i_short)
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
    
    //检查imm.val和rm.val的最高位（第8位），如果该位为1，说明操作数为负数或具有符号
    //通过将imm.val和rm.val与0xFFFFFF00进行按位或运算，确保高24位也被设置为1，从而进行正确的符号扩展
    if((rm.val & 0x80) != 0){
        rm.val = 0xFFFFFF00 | rm.val;
    }
    if((imm.val & 0x80) != 0){
        imm.val = 0xFFFFFF00 | imm.val;
    }
    
    rm.val = alu_and(imm.val, rm.val, data_size);
    operand_write(&rm);
    
    return len + 1;
}

make_instr_func(and_rm_i_v_short)
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

    rm.val = alu_and(imm.val, rm.val, data_size);
    operand_write(&rm);
    //函数最后返回len + 1，即整个指令的长度（以字节为单位）。这里，len代表MOD R/M字节和其他可能的指令字节，+ 1代表立即数操作数占据的空间，因为立即数是8位，即1字节
    return len + 1;
}

make_instr_func(and_rm_r_v){
    OPERAND rm, r;
    
    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    
    rm.val = alu_and(r.val, rm.val, data_size);
    //将计算得到的新值赋给rm.val，结果写回到原来从内存读取rm值的位置
    operand_write(&rm);
    //函数最后返回len，即整个指令的长度（以字节为单位）
    return len;
}

make_instr_func(and_rm_r_short)
{
    OPERAND rm, r;
    
    rm.data_size = 8;
    r.data_size = 8;
    int len = 1;
    //解析MOD R/M字节并确定操作数
    len += modrm_r_rm(eip + 1, &r, &rm);
    uint8_t k = 32 - data_size;
    
    operand_read(&rm);
    operand_read(&r);
    //只保留最低8位的值。
    //使用sign_ext函数对rm.val和r.val进行符号扩展
    rm.val = alu_and(sign_ext(rm.val & (0xFFFFFFFF >> k), data_size), sign_ext(r.val & (0xFFFFFFFF >> k), data_size), data_size);
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(and_r_rm_short)
{
    OPERAND rm, r;
    
    rm.data_size = 8;
    r.data_size = 8;
    uint8_t k = 32 - data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    
    r.val = alu_and(sign_ext(rm.val & (0xFFFFFFFF >> k), data_size), sign_ext(r.val & (0xFFFFFFFF >> k), data_size), data_size);
    //将计算得到的新值赋给r.val，结果将被写回到寄存器r中
    operand_write(&r);
    //函数最后返回len，即整个指令的长度（以字节为单位）。这包括MOD R/M字节和任何额外的寻址模式字节的总长度。
    return len;
}

make_instr_func(and_i2a_v){
    OPERAND r,imm;

    r.data_size = data_size;
    r.type = OPR_REG;
    r.addr = 0x0;

    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = data_size;

    operand_read(&r);
    operand_read(&imm);
    r.val = alu_and(r.val, imm.val, data_size);
    operand_write(&r);

    return 1 + data_size / 8;
}

make_instr_func(and_rm_i_v)
{
    OPERAND rm,imm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = data_size;
    
    operand_read(&rm);
    operand_read(&imm);
    
    rm.val = alu_and(imm.val, rm.val, data_size);
    operand_write(&rm);
    
    return len + data_size / 8;
}



make_instr_func(and_i2a_b)
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
    r.val = alu_and(sign_ext(imm.val & (0xFFFFFFFF >> k), data_size), sign_ext(r.val & (0xFFFFFFFF >> k), data_size), data_size);
    operand_write(&r);
    
    return 2;
}

make_instr_func(and_rm2r_v)
{
    OPERAND rm, r;

    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);

    operand_read(&rm);
    operand_read(&r);

    r.val = alu_and(rm.val, r.val, data_size);

    operand_write(&r);

    return len;
}
