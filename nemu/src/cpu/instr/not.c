#include "cpu/instr.h"
/*
Put the implementations of `not' instructions here.
*/

make_instr_func(not_rm_b)
{
    OPERAND rm;
    
    rm.data_size = 8;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    //使用按位取反运算符~对rm.val进行按位取反操作。在C/C++中，~rm.val直接实现了数值的每一位取反，对于8位的无符号整数，这将对每一位进行1和0的交换
    rm.val = ~rm.val;
    
    operand_write(&rm);
    
    return len;
}


make_instr_func(not_rm_v)
{
    OPERAND rm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    //使用按位取反运算符~对rm.val进行按位取反操作。在C/C++中，~rm.val直接实现了数值的每一位取反 
    rm.val = ~rm.val;
    
    operand_write(&rm);
    
    return len;
}
