#include "cpu/instr.h"
/*
Put the implementations of `inc' instructions here.
*/

make_instr_func(inc_rm){
    OPERAND rm;
    //初始化操作数和指令长度 
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    
    int temp = 0;
    if(cpu.eflags.CF == 1){
        temp = 1;
    }
    //调用alu_add函数，将1（即加数值）和rm.val作为参数，以及data_size作为数据大小参数。alu_add函数执行加法操作，实质上是将rm.val递增1
    rm.val = alu_add(1, rm.val, data_size);
    //处理进位标志（CF）
    if(temp == 1){
        cpu.eflags.CF = 1;
    }
    else{
        cpu.eflags.CF = 0;
    }
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(inc_r){
    OPERAND r;
    
    r.addr = opcode & 0x7;
    r.data_size = data_size;
    r.type = OPR_REG;
    
    operand_read(&r);
    
    int temp = 0;
    if(cpu.eflags.CF == 1){
        temp = 1;
    }
    
    r.val = alu_add(1, r.val, data_size);
    //如果cpu.eflags.CF原本就是1，则保持其为1。如果cpu.eflags.CF原本为0，则将其设置为0
    if(temp == 1){
        cpu.eflags.CF = 1;
    }
    else{
        cpu.eflags.CF = 0;
    }
    //将计算得到的新值赋给r.val，结果写回到寄存器r中
    operand_write(&r);
    
    return 1;
}
