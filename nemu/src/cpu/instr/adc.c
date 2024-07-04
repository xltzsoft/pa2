#include "cpu/instr.h"
/*
Put the implementations of `adc' instructions here.
*/
make_instr_func(adc_rm_r)
{
    OPERAND rm, r;
    //初始化两个OPERAND类型的变量rm和r，它们分别代表寄存器和内存操作数
    rm.data_size = data_size;
    r.data_size = data_size;
    //设置rm和r的数据大小等于data_size
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    //计算指令长度
    operand_read(&rm);
    operand_read(&r);
    //读取rm和r操作数的值
    rm.val = alu_adc(r.val, rm.val, data_size);
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(adc_ra_i_short)
{
    OPERAND r,imm;
    //imm代表立即数操作数
    r.data_size = 8;
    r.type = OPR_REG;
    r.addr = 0x0;
    uint8_t k = 32 - data_size;
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = 8;
    //使用operand_read函数从内存读取rm的值，以及从寄存器读取r的值
    operand_read(&r);
    operand_read(&imm);
    r.val = alu_adc(sign_ext(imm.val & (0xFFFFFFFF >> k), data_size), sign_ext(r.val & (0xFFFFFFFF >> k), data_size), data_size);
    //使用sign_ext函数对两个操作数进行符号扩展，确保保留正确的符号位
    operand_write(&r);
    
    return 2;
}

make_instr_func(adc_rm_r_short)
{
    OPERAND rm, r;
    //设置rm和r的数据大小为8位
    rm.data_size = 8;
    r.data_size = 8;
    int len = 1;
    uint8_t k = 32 - data_size;
    len += modrm_r_rm(eip + 1, &r, &rm);
    //调用modrm_r_rm函数，解析MOD R/M字节，确定寄存器和内存操作数的具体信息，并返回它们所占的字节数
    operand_read(&rm);
    operand_read(&r);
    
    rm.val = alu_adc(sign_ext(r.val & (0xFFFFFFFF >> k), data_size), sign_ext(rm.val & (0xFFFFFFFF >> k), data_size), data_size);
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(adc_rm_i_v_short)
{
    OPERAND rm,imm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    //在MOD R/M字节之后的第一个字节处，数据大小设为8位
    imm.data_size = 8;
    
    operand_read(&rm);
    operand_read(&imm);
    //如果imm.val的最高有效位（第8位）为1，说明立即数为负数。
    //将imm.val与0xFFFFFF00进行按位或运算，确保高24位也被设置为1，从而正确地进行符号扩展。
    if((imm.val & 0x80) != 0){
        imm.val = 0xFFFFFF00 | imm.val;
    }
    
    rm.val = alu_adc(imm.val, rm.val, data_size);
    
    operand_write(&rm);
    
    return len + 1;
}


make_instr_func(adc_i2rm_v)
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
    rm.val = alu_adc(imm.val, rm.val, data_size);
    operand_write(&rm);
    
    return len + data_size / 8;
    //函数最后返回整个指令的长度（以字节为单位）
    //包括MOD R/M字节和立即数操作数的字节。由于data_size是以位为单位的，将其除以8转换为字节。
}

make_instr_func(adc_i2rm_b)
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
    //将扩展后的imm.val和rm.val作为参数，以及data_size（在这里是8位）作为数据大小参数。
    rm.val = alu_adc(imm.val, rm.val, data_size);
    operand_write(&rm);
    
    return len + 1;
    //函数最后返回整个指令的长度（以字节为单位），包括MOD R/M字节和立即数操作数的字节
}

make_instr_func(adc_rm2r_b)
{
    OPERAND rm, r;
    
    rm.data_size = 8;
    r.data_size = 8;
    uint8_t k = 32 - data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    //rm.val和r.val保留最低8位的值。
    r.val = alu_adc(sign_ext(rm.val & (0xFFFFFFFF >> k), data_size), sign_ext(r.val & (0xFFFFFFFF >> k), data_size), data_size);
    
    operand_write(&r);
    
    return len;
}

make_instr_func(adc_rm2r_v)
{
    OPERAND rm, r;
    
    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    //调用modrm_r_rm函数，传入eip + 1（当前指令指针后的第一个字节）以及指向r和rm的指针
    operand_read(&rm);
    operand_read(&r);
    
    r.val = alu_adc(rm.val, r.val, data_size);
    
    operand_write(&r);
    
    return len;
}


make_instr_func(adc_i2a_v)
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
    r.val = alu_adc(imm.val, r.val, data_size);
    operand_write(&r);
    
    return 1 + data_size / 8;
    //函数最后返回整个指令的长度（以字节为单位）。
    //1代表MOD R/M字节或其他潜在的指令前缀字节，data_size / 8转换data_size从位到字节，以计算立即数操作数占据的空间
}
