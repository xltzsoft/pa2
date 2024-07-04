#include "cpu/instr.h"
/*
Put the implementations of `lea' instructions here.
*/

make_instr_func(lea)
{
    OPERAND  m,r;
    //初始化操作数和指令长度 
    m.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &m);
    //解析MOD R/M字节并确定操作数
    //计算有效地址
    r.val = m.addr;
    
    operand_write(&r);
    
    return len;
}
