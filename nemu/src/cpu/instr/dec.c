#include "cpu/instr.h"
/*
Put the implementations of `dec' instructions here.
*/

make_instr_func(dec_r){
    OPERAND r;
    //初始化操作数 
    r.addr = opcode & 0x7;
    r.data_size = data_size;
    r.type = OPR_REG;
    
    operand_read(&r);
    //准备临时变量temp，保存进位标志（CF）的原始状态。
    //检查cpu.eflags.CF的值，如果为1，则设置temp为1；否则为0，保持CF标志的一致性。
    //调用alu_sub函数，将1减去r.val，以及data_size作为数据大小参数。这里alu_sub函数实质上执行了减一操作，但由于DEC指令的特性，这里的参数顺序与常规减法相反，以实现递减效果。
    int temp = 0;
    if(cpu.eflags.CF == 1){
        temp = 1;
    }  
    
    r.val = alu_sub(1, r.val, data_size);
    //根据temp的值更新cpu.eflags.CF。由于DEC指令执行后，CF标志通常被清零，除非在特殊情况下需要保持其状态不变
    if(temp == 1){
        cpu.eflags.CF = 1;
    }
    else{
        cpu.eflags.CF = 0;
    }
    
    operand_write(&r);
    
    return 1;
}

make_instr_func(dec_rm){
    OPERAND rm;
    
    rm.data_size = data_size;

    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    
    int temp = 0;
    if(cpu.eflags.CF == 1){
        temp = 1;
    }
    
    rm.val = alu_sub(1, rm.val, data_size);
    //处理进位标志（CF）
    if(temp == 1){
        cpu.eflags.CF = 1;
    }
    else{
        cpu.eflags.CF = 0;
    }
    //将计算得到的新值赋给rm.val，结果将写回到原来从内存读取rm值的位置
    operand_write(&rm);
    
    return len;
}
