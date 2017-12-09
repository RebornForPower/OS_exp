/*
 *  RiOS/kernel/init/interrupt/interrupt.h
 *
 *  Copyright (C) 2017  Curie （邱日）
 */
/*本程序和RiOS/kernel/main_gas.S联系紧密
 *这里的函数都是中断处理函数，由main_gas.S中那些汇编语言写的函数调用
 *这里函数的特点 void hello(int *esp)
 */
 void _8253timer_interrupt(int *esp);//NOTE!!每次8253发生时钟中断，就调用它
 //进程调度也在这个部分调度，这是调用频率最高的函数了。
 //function defined in main_gas.S
 /*gas_handlers---------*/
 /*在gas写成的汇编函数里call中断服务程序，gas_handlers主要作用：
  *push ;call InterruptServiceFunc ;pop
  */
 /*---interrupt service functions----------*/
 /*first,add a record in idt,define the address of gas_handler_myinterrupt1 in main_gas.S
  *then,in gas_handler_myinterrupt1 push,call myinterrupt1,pop
  *finally the CPU will run the code in those interrupt service function.
  */
/*TODO :add more Interrupt service functions and add records in interrupt descriptor table*/
void Hello_RiOS(int *esp);/*显示一些欢迎信息，体现如何写显存*/
void myinterruptProcessA(int *esp);
void myinterruptProcessB(int *esp);
