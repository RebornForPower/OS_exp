/*
 *  RiOS/kernel/main_gas.S
 *
 *  Copyright (C) 2017  Curie （邱日）
 */
/*本程序和RiOS/kernel/init/interrupt/interrupt.h,interrupt.c联系紧密
 *在这里的函数call的那些中断处理函数的形式是void hello(int *esp),都在上述地方声明和定义
 */
.global Ri_load_gdt
.global Ri_load_idt
.global Ri_load_ldt

.global gas_Handle_8253timer /*时钟中断处理函数 NOTE!!很重要*/

.global gas_ltr
.global gas_Handle_myinterruptProcessA
.global gas_Handle_myinterruptProcessB


.code32
Ri_load_gdt:		/*void Ri_load_gdt(int length,int address){*///stack
                mov 4(%esp) ,%ax
                mov %ax,6(%esp)
                lgdt 6(%esp)
                ret//这里用ret,ret 和 iret 还不太一样
Ri_load_idt:
                mov 4(%esp) ,%ax
                mov %ax,6(%esp)
                lidt 6(%esp)//之前这地方lidt错写成lgdt了，找了半天
                ret
Ri_load_ldt:
                mov 4(%esp) ,%ax
                mov %ax,6(%esp)
                lldt 6(%esp)//之前这地方lidt错写成lgdt了，找了半天
                ret

#ljmp $section, $offset
#错误！
# Ri_farjmp:		//; void farjmp(int eip, int cs);//远跳转 ljmp
#       ljmp    *4(%esp)
#       ret
#       //; eip, cs //从Linux0.11 sched.h 宏汇编ljmp *%0...得到启发
# //C语言调用 usage: Ri_farjmp(eip的值，cs的值); Ri_farjmp(0,4>>3);



gas_Handle_8253timer:
                pushw %es
                pushw %ds
                pushal
                movl %esp,%eax
                pushl %eax
                movw %ss,%ax
                movw %ax,%ds
                movw %ax,%es//ensure ss=ds=es
                //call myinterruptProcessB
                call _8253timer_interrupt
/*_8253timer_interrupt定义在RiOS/init/interrupt/interrupt.h中
 *时钟中断的处理函数可能是调用频率最高的函数了，它的声明void _8253timer_interrupt(int *esp)
 *此类函数参数列表要注意一下，都是(int *esp)
 */
                popl %eax
                popal
                popw %ds
                popw %es
                iret

gas_Handle_myinterruptProcessA:
                pushw %es
                pushw %ds
                pushal //no pushad,but pusha in Ubuntu
                //movl %eax,%esp
                mov %esp,%eax
                pushl %eax
                movw %ss,%ax
                movw %ax,%ds
                movw %ax,%es//ensure ss=ds=es
                call myinterruptProcessA//定义在RiOS/init/interrupt/interrupt.h
//call的这类函数参数列表要注意一下，都是(int *esp)，例如void hello(int *esp)
                popl %eax
	        popal//popal //popad
                popw %ds
                popw %es
                iret

gas_Handle_myinterruptProcessB:
                pushw %es
                pushw %ds
                pushal
                movl %esp,%eax
                pushl %eax
                movw %ss,%ax
                movw %ax,%ds
                movw %ax,%es//ensure ss=ds=es for Clang
                //call myinterruptProcessB
                call Hello_RiOS
                popl %eax
                popal
                popw %ds
                popw %es
                iret

gas_ltr://gas_ltr(int tr)
                ltr 4(%esp)
                ret
