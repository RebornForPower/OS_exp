/*
 *  RiOS/kernel/init/interrupt/interrupt.c
 *
 *  Copyright (C) 2017  Curie （邱日）
 */
/*-------interrupt-function----------*/
/*TODO :add more Interrupt service functions and add records in interrupt descriptor table*/
/*NOTE!! IDT is important */
void Hello_RiOS(int *esp)
{
		//console_cls();
		int keydata;
		outb(PIC0_OCW2, 0x61);//要把IRQ-01接收信号结束的信息通知给PIC
		#define KEYDATAPort 0x60
		keydata = inb(KEYDATAPort);
/*一定要把数取走，中断响应
 *两个事情，第一给8259发送接受信号，第二把数取走。
 *这两个只要一个没有做就不行
 */
//   char str[]="/*------Welcome to RiOS!------*/~\
// Copyright (C) 2017 Curie   ^";
 		int i=0;//~ => \n ,^ => \0
		int ch;
// 	for(i=0;str[i]!='^';i++){
// 	ch=str[i];if(ch>=127|ch<32)ch=32;
// 	Ri_putchar(ch);
//   }
/*好像puchcar(65)可以，尝试把char转为int,这样就可以输出了
 *中断服务子程序要体现关中断，要给8259一个回应，不然它被阻塞了
 *NOTE!!!不能return;
 *Ri_putchar_colorful(95,0b10000000);
 *考虑用写显存的方法控制光标（‘_’ascii字符95,把它属性设为闪烁看看）
 *80×25 显示效果 王爽《汇编语言》	|7				 6 5 4 	  3		 2 1 0
 *														|BL(闪烁)		R G B  高亮		R G B
 */
  		unsigned char *pVRAM=(char*)0xb8000;
 /* 		#define _setTcolor pVRAM[(80*SCREEN_Y+SCREEN_X)*2+1]=0b11000010 */
  // _setTcolor,Ri_putchar(58),_setTcolor,Ri_putchar(45),_setTcolor,Ri_putchar(41);
		char str2[]="~[root@localhost]# ^";
		/* #define Black_greenT 0b00000010*/
		for(i=0;str2[i]!='^';i++){
			ch=str2[i];//if(ch>=127|ch<32)ch=32;
			Ri_putchar_colorful(ch,Black_greenT);//好像puchcar(65)可以，尝试把char转为int,这样就可以输出了
  		}
  		return;
}

/*我的电脑开机后一直打印A,先把这两个中断给注释掉*/
void myinterruptProcessA(int *esp){
		// Ri_putchar('A');
		// Sleep(1);
		return;
}
/*我的电脑开机后一直打印A,先把这两个中断给注释掉*/
void myinterruptProcessB(int *esp){
		//while(1)Ri_putchar('B');
		// outb(PIC0_OCW2, 0x61);
		// int keydata;
		// #define KEYDATAPort 0x60
		// keydata = inb(KEYDATAPort);
		// //一定要把数取走，不然会阻塞
		// Ri_putchar(keydata);
		// Sleep(1);
		return;
}
