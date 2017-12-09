/*
 *  RiOS/init/initialize.c
 *
 *  Copyright C) 2017  Curie （邱日）
 */

int initialize(){
/*first, initialize gdt(global descriptor table) with zero, and set it(add records and lgdt ect)
 *then, initialize idt(global interrupt table) with zero,and set it(add records and lidt ect)
 *after that,send ICW to 8259 port ,NOTE!!DONOT forget to sti!!! (enable interrupt request)
 * NOTE!! DONOT forget to enable keyboard interrupt Mouse interrupt and 8253 interrupt after the
 *initialization of 8253!!!
 */
		init_gdt();
		set_gdt();
		init_idt();
		set_idt();
		init_8259();
//gdt,idt,8259 ready.
		Ri_sti();//enable interrupt
		init_8253();//光初始化还没有用，要8259允许8253时钟中断请求
/*---------TODO:Timer---------------*/
/*1.初始化设定8253
 *2.设定idt的一条记录
 *3.编写每次8253中断的处理程序。
 *和键盘中断一样，在8253计时器响应的中断处理程序代码里
 *也必须给外面8259发信号，中断已接受，不然又阻塞。
 *4.必须先允许8253中断
 */
/*---------Timer  :work done --------*/
		 outb(PIC0_IMR, 0xf9);//1111 1001  irq 1 2打开 因为keyboard是irq 1
/*由于出现了一些问题，暂时只允许键盘，不允许鼠标*/
/*	KeyboardMouseintEnable();*/
//_8253timer_interrupt_enable();//NOTE!! important!!
/*注意，为了我的task_struct进程队列别搞乱，我先暂时不开时钟中断，
 *等我的队列头都初始化好了我再开8253时钟中断
 */
/*2017/10/25 (现已解决):经过测试，是可以中断了。无论是10s之前还是10s之后，都可以中断
 *但有个问题，就是只能中断一次，想来应该是发出中断请求，收到
 *中断请求后响应，关中断，但接下来没有再把中断打开，所以接下来的中断响应不了了
 *我的8253也要再设置
 *2017/10/27 ADD:微机原理白学了，忘了要 NOTE!!:“中断响应”
 *不然它一直以为还没有得到回应。
 *在中断处理程序中要有outb(PIC0_OCW2, 0x61);  要把IRQ-01接收信号结束的信息通知给PIC
 */
		init_mem_table(RiOSmm);/*见RiOS/kernel/mm/memory.c*/
		/*其中RiOSmm是用于内存管理的全局结构体指针，在main.c中定义*/
		return 0;
}
