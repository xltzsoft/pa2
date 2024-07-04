#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/

make_instr_func(leave)
{
    OPERAND  esp, ebp,r;
    //初始化操作数 
    //恢复ESP寄存器的值
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    //读取EBP寄存器的值
    ebp.data_size = 32;
    ebp.type = OPR_REG;
    ebp.addr = 0x5;
    //从堆栈中读取旧的EBP值
    operand_read(&ebp);
    //更新ESP指针
    esp.val = ebp.val;
    
    r.data_size = 32;
    r.type = OPR_MEM;
    r.sreg = SREG_SS;
    r.addr = esp.val;
    
    operand_read(&r);
    
    esp.val += 4;
    
    ebp.data_size = 32;
    ebp.val = r.val;
    
    operand_write(&esp);
    operand_write(&ebp);
    
    return 1;
}
