#include "cpu/instr.h"
/*
Put the implementations of `xor' instructions here.
*/

make_instr_func(xor_rm2r_v)
{
    OPERAND rm, r;
    
    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    
    r.val = alu_xor(rm.val, r.val, data_size);
    
    operand_write(&r);
    
    return len;
}

make_instr_func(xor_rm2r_b)
{
    OPERAND rm, r;
    
    rm.data_size = 8;
    r.data_size = 8;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    uint8_t k = 32 - data_size;
    r.val = alu_xor(sign_ext(rm.val & (0xFFFFFFFF >> k), data_size), sign_ext(r.val & (0xFFFFFFFF >> k), data_size), data_size);
    
    operand_write(&r);
    
    return len;
}


make_instr_func(xor_r2rm_v)
{
    OPERAND rm, r;

    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);

    operand_read(&rm);
    operand_read(&r);

    rm.val = alu_xor(r.val, rm.val, data_size);

    operand_write(&rm);

    return len;
}

make_instr_func(xor_r2rm_b)
{
    OPERAND rm, r;
    
    rm.data_size = 8;
    r.data_size = 8;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    uint8_t k = 32 - data_size;
    rm.val = alu_xor(sign_ext(r.val & (0xFFFFFFFF >> k), data_size), sign_ext(rm.val & (0xFFFFFFFF >> k), data_size), data_size);
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(xor_i2a_v)
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
    r.val = alu_xor(imm.val, r.val, data_size);
    operand_write(&r);
    
    return 1 + data_size / 8;
}

make_instr_func(xor_i2a_b)
{
    OPERAND r,imm;
    
    r.data_size = 8;
    r.type = OPR_REG;
    r.addr = 0x0;
    
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = 8;
    
    operand_read(&r);
    operand_read(&imm);
    uint8_t k = 32 - data_size;
    r.val = alu_xor(sign_ext(imm.val & (0xFFFFFFFF >> k), data_size), sign_ext(r.val & (0xFFFFFFFF >> k), data_size), data_size);
    operand_write(&r);
    
    return 2;
}

make_instr_func(xor_i2rm_v)
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
    rm.val = alu_xor(rm.val, imm.val, data_size);
    operand_write(&rm);
    
    return len + data_size / 8;
}


make_instr_func(xor_i2rm_b)
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
    
    rm.val = alu_xor(imm.val, rm.val, data_size);
    operand_write(&rm);
    
    return len + 1;
}

make_instr_func(xor_rm_i_v_short)
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

    rm.val = alu_xor(imm.val, rm.val, data_size);

    operand_write(&rm);

    return len + 1;
}
