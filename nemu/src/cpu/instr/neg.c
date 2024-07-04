#include "cpu/instr.h"
/*
Put the implementations of `neg' instructions here.
*/

make_instr_func(neg_rm_b)
{
    OPERAND rm;
    
    rm.data_size = 8;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    
    if(rm.val == 0){
        cpu.eflags.CF = 0;
    }    
    else{
        cpu.eflags.CF = 1;
    }
    //将rm.val的值取反，即乘以-1。在C/C++中，-rm.val直接实现了数值的取反。
    //rm.val是一个8位的无符号整数，在C/C++中，-rm.val实际上是计算了rm.val的二进制补码，相当于对其进行取反后再加1
    rm.val = -rm.val;
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(neg_rm_v)
{
    OPERAND rm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    //如果rm.val为0，将进位标志（CF）设置为0；否则，将进位标志（CF）设置为1。
    //当操作数为0时，取反操作不会产生进位，因此CF应该为0；否则，取反操作会产生进位，因此CF应该为1
    if(rm.val == 0){
        cpu.eflags.CF = 0;
    }    
    else{
        cpu.eflags.CF = 1;
    }
    rm.val = -rm.val;
    
    operand_write(&rm);
    
    return len;
}
