/*
 *  RiOS/include/asm/8259_8253.h
 *
 *  Copyright (C) 2017  Curie （邱日）
 */
void init_8259();
void KeyboardMouseintEnable();
void init_8253();
void _8253timer_interrupt_enable();
/*---init 8259 init 8253  END------------*/
 /*---Programmable Interrupt Controller------*/
 /*outb from Linux kernel include/asm/io.h
  *#define outb(value,port) \
  *__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))
  *WARNNING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  *这里要特别注意，GNU format Assembly 和 MASM的操作数位置正好相反
  *linux 里的gas是 outb value,port 即 outb %al,%dx
  *而MASM的汇编语法是 out port,value,即 out dx,al
  *我因此困在这个地方，迟迟不能实现中断。(我把Linux outb 操作数顺序换一换)
  */
 #define outb(port,value) \
  	__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))

 #define inb(port) ({ \
  	unsigned char _v; \
  	__asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port)); \
  	_v; \
  	})
 /*---8259 Port address-------------*/
 #define PIC0_OCW2		0x0020
 #define PIC0_IMR		0x0021

 #define PIC0_ICW1		0x0020
 #define PIC0_ICW2		0x0021
 #define PIC0_ICW3		0x0021
 #define PIC0_ICW4		0x0021


 #define PIC1_OCW2		0x00a0
 #define PIC1_IMR		0x00a1

 #define PIC1_ICW1		0x00a0
 #define PIC1_ICW2		0x00a1
 #define PIC1_ICW3		0x00a1
 #define PIC1_ICW4		0x00a1
 /*---8259 Port address END------------*/

 /*----8253 timer Port address-------------*/
 #define PIT_CTRL	0x0043
 #define PIT_CNT0	0x0040
 /*----8253 timer Port address END----------*/

 /*---init 8259 init 8253-------------*/
 /*NOTE!!:after the initialization of 8259,8253,
  *We should enable interrupt,otherwise it wont't work.
  */
  /*---------gas--------------------------------*/
  /* inline assembly and entern GNU format Assembly Code from maingas.S*/
  //void Ri_cli();//close interrupt
  //void Ri_sti();//enable interrupt
  //inline Assembly Learned from Linux0.11
  #define Ri_sti() __asm__ ("sti"::)//允许中断
  #define Ri_cli() __asm__ ("cli"::)//关中断
  /* my original cli ,sti in C languange
  void Ri_cli(){//close
  			#define Ri_cli_gas() __asm__ ("cli"::)
  			//这里是单个指令不能加ret,不然导致死循环
   			Ri_cli_gas();
  }//~
  void Ri_sti(){//open
  		 #define Ri_sti_gas() __asm__ ("sti"::)
  		 //这里是单个指令不能加ret,不然导致死循环
  		 Ri_sti_gas();
  }//~
  */
  //Alternatively ,I learned this from Linux0.11
  //#define Ri_sti() __asm__ ("sti"::)
  //#define Ri_cli() __asm__ ("cli"::)
/*查看中断向量表：
 *https://max.book118.com/html/2016/0525/43960342.shtm
 *8259A中断类型码08h时，功能是计时器
 *计时器timer对应8号中断向量,但是那是BIOS功能调用的向量号了
 *但我现在在保护模式，当然不能用BIOS中断，因此也无所谓了
 *正常把时钟中断向量设在0x20h
 */
