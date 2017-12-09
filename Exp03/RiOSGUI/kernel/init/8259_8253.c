/*
 *  RiOS/init/8259_8253.c
 *
 *  Copyright (C) 2017  Curie （邱日）
 */
 /*---Programmable Interrupt Controller definition------*/
void init_8259()
{
 	  outb(PIC0_IMR, 0xff);
 	  outb(PIC1_IMR, 0xff);//向port 0x21 ,0xa1写0xff　　禁止所有的irq中断

 	  //对master pic进行设置
 	  outb(PIC0_ICW1, 0x11);//设置产生中断的方式为边沿触发　　　							0x20
 	  outb(PIC0_ICW2, 0x20);//设置中断号的基地址，原来是irq 0x0到0x7,现在基地址是0x20，所以中断号变为int20到int27     0x21
 	  outb(PIC0_ICW3, 1<<2);//对master pic的bit2进行写１操作，设置slave pic由irq2连接				0x21
 	  outb(PIC0_ICW4, 0x01);// no buffer mode								0x21

 	  outb(PIC1_ICW1,  0x11);//edge trigger mode
 	  outb(PIC1_ICW2,  0x28);//设置中断号的基地址，中断号变为int28 int0x2f 接收
 	  outb(PIC1_ICW3,  2   );//pic0 bit2
 	  outb(PIC1_ICW4,  0x01);//no buffer mode

 	  outb(PIC0_IMR,  0xfb);//1111 1011 打开pic1的中断请求位 port 0x21
 	  outb(PIC1_IMR,  0xff);//禁止pic１芯片的所有请求　　port 0xa1
          return;
}//~

 //赵炯《Linux内核完全注释修正版v3.0》
 /*
  * #define	_8253_Way3bin   0x36 //方式3：方波发生器,二进制计数
  * #define _8253_WritePort 0x43// 8253控制字寄存器写端口
 	//在8086最小系统中，1片8253的通道0、1、2端口地址为1240H、1242H、1244H
  * #define _8253_Channel_0 0x40//8253通道0端口
  */
 void init_8253()
 {
 //锁存器latch 赋值11930
 	       #define LATCH 11930 //interrupt request every 10ms
 /*Latch为计数器的初值，1193180/100=11930,频率100HZ,即莓10ms一次中断请求
  *lATCH的值较大11930=0b 0010 1110 1001 1010,要分两次写入端口,每次8位
  *先写低位，再写高位
  */
 /*
  *outb(_8253_WritePort,	_8253_Way3bin);
  *outb(_8253_Channel_0,LATCH&0xff);//低8位，按位与&符号不能写错
  *按位与：a&b是把a和b都转换成二进制数然后再进行与的运算；
  *逻辑与：a&&b就是当且仅当两个操作数均为 true时，其结果才为 true；只要有一个为零，a&&b就为零。
  *a&b  9&8=1001 & 1000=0b1000,但9&&8=1
  *NOTE!!WARNNING!!!上次我把按位与&错写成逻辑与&&,找了一天bug才找出来
  *outb(_8253_Channel_0,LATCH>>8);//高8位
  */
         	outb(PIT_CTRL, 0x34);
         	outb(PIT_CNT0, 0x9c);
         	outb(PIT_CNT0, 0x2e);
 	return;
 }//~

 void KeyboardMouseintEnable()
 {
 		outb(PIC0_IMR, 0xf9);//1111 1001  irq 1 2打开 因为keyboard是irq 1
 		//0xf8是连同PIT也允许，我现在0xf9尚未允许8253的时钟中断，要写0xf8允许8253
 		outb(PIC1_IMR, 0xef);//1110 1111  irq 12打开　mouse是irq 12  所以要把pic 1 pic 2的芯片中断响应位打开。
 		return;
 }//~

 void _8253timer_interrupt_enable()
 {
 	       outb(PIC0_IMR, 0xf8);
               return;
 }
