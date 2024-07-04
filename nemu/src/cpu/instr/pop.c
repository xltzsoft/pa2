#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/

make_instr_func(pop_r_v){
    
    OPERAND r,esp;
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    //设置r的数据大小为32位，类型为内存操作数，段寄存器为堆栈段寄存器，地址设为esp.val，即堆栈顶部的地址 
    operand_read(&esp);
    
    r.data_size = 32;
    r.type = OPR_MEM;
    r.sreg = SREG_SS;
    r.addr = esp.val;
    
    operand_read(&r);
    //将ESP的值增加4，这是因为弹出一个32位的数据会使得堆栈顶部向高地址移动4个字节
    esp.val += 4;
    //设置r的地址为opcode & 0x7，这里opcode是操作码的一部分，& 0x7的操作是为了提取出低位的三位，用于确定目标寄存器
    r.addr = opcode & 0x7;
    r.type = OPR_REG;
    r.data_size = data_size;
    
    operand_write(&esp);
    operand_write(&r);
    
    return 1;
}


make_instr_func(popa){
    OPERAND r;
    r.data_size = 32;
    r.sreg = SREG_SS;
    r.type = OPR_MEM;
   
    int k=0;
    for(;k<3;k++)
    {
        //设置r的地址为当前ESP寄存器的值，即堆栈顶部的地址
        r.addr = cpu.esp;
	//读取堆栈顶部的数据，即从内存读取一个32位的数据值
        operand_read(&r);
	//根据循环次数，将读取的数据写入不同的寄存器中（EDI、ESI、EBP、跳过一次、EBX、EDX、ECX、EAX）
        cpu.edi = r.val;
	//更新ESP寄存器的值，使其增加4，反映堆栈顶部向高地址移动4个字节，即弹出了一个32位的数据
        cpu.esp += 4;
    }

    cpu.esp += 4;
    //在第三个寄存器（EBP）和第四个寄存器（EBX）之间，有一个未使用的寄存器位置。代码中通过cpu.esp += 4;直接跳过了这一位置，没有读取数据，也没有写入任何寄存器
    for(;k>=0;k--)
    {
       r.addr = cpu.esp;
       operand_read(&r);
       cpu.ebx = r.val;
       cpu.esp += 4;
    }
    
    return 1;
}
