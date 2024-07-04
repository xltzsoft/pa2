#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.
*/

make_instr_func(ret_near){
    
    OPERAND esp,ip;
    
    esp.addr = 0x4;
    esp.data_size = 32;
    esp.type = OPR_REG;
    
    operand_read(&esp);
    //设置ip的数据大小为32位，类型为内存操作数（OPR_MEM），段寄存器为堆栈段寄存器（SREG_SS），地址设为esp.val，即堆栈顶部的地址 
    ip.data_size = 32;
    ip.type = OPR_MEM;
    ip.sreg = SREG_SS;
    ip.addr = esp.val;
    
    operand_read(&ip);
    //将ESP的值增加4，反映弹出一个32位的返回地址后堆栈的变化
    esp.val += 4;
    
    cpu.eip = ip.val;
    
    operand_write(&esp);
    
    return 0;
}

make_instr_func(ret_near_i)
{
    OPERAND esp,ip,imm;
    
    esp.addr = 0x4;
    esp.data_size = 32;
    esp.type = OPR_REG;
    //读取ESP寄存器的值
    operand_read(&esp);
    //准备读取堆栈顶部的返回地址
    ip.data_size = 32;
    ip.type = OPR_MEM;
    ip.sreg = SREG_SS;
    ip.addr = esp.val;
    //从堆栈顶部读取返回地址，即下一个指令的地址
    operand_read(&ip);
    
    imm.data_size = 16;
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    
    operand_read(&imm);
    
    esp.val += 4;
    //将读取的返回地址赋值给CPU的eip字段，控制程序流返回到调用者处
    cpu.eip = ip.val;
    
    esp.val += imm.val;
    //将更新后的ESP值写回ESP寄存器，反映堆栈状态的变化
    operand_write(&esp);
    
    return 0;
}
