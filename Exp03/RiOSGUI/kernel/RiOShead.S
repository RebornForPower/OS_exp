/*
 *  RiOS/kernel/bootloader.S
 *
 *  Copyright (C) 2017  Curie （邱日）
 */

#define BootGasStart 0x8000
#define HeadGasStart 0x10000
.code16
.global start
//对照《Linux内核完全注释修正版v3.0》图4.40
start:
	xor %ax,%ax
	mov %ax,%ds
	mov %ax,%es
	mov %ax,%ss
/*------320*200 VIDEO mode,simple xwindow--*/
/*	mov $0x13,%al
 *	xor %ah,%ah
 *	int $0x10	//main.c _xwindowRiOSmain
 */
/*int 0x13 or int 0x03 ? This is a choice. */
/*------80*25  TEXT mode,default--*/
	// mov $0x03,%al
	// xor %ah,%ah
	// int $0x10//main.c RiOSmain
/*NOTE!!2017.12.3 这里特别要注意，和第一次实验进入的是80×25的文本显示模式不同
 *本次实验要进入图形界面模式，故不应用$0x03BIOS的调用
 *而应当用$0x13的BIOS功能调用，进入320×200分辨率的video mode,
 *使用彩色图形界面。
 */
	mov $0x13,%al
	xor %ah,%ah
	int $0x10	//main.c _xwindowRiOSmain


/*------To do:enable address line A20-----*/
/*oxff=>port 0x21,port 0xa1*/
	movb $0xff,%al
	outb %al,$0x21
	nop
	outb %al,$0x21
	cli //diable interrupt
	inb $0x64,%al
//reference:----http://kernelx.weebly.com/a20-address-line.html


/*-------enable CR0 bit0,Real mode->Protected Mode*/
	lgdt gdt_info
	movl %cr0,%eax
	andl $0x7fffffff,%eax
	orl $0x01,%eax
	movl %eax,%cr0

/* |PG(bit31)				PE(bit0)|
 * PE(BIT0):控制分段管理机制,0处理器运行于实模式,1为保护模式
 * PG(BIT31):控制分页管理机制,0禁用分页管理机制(此时线性地址等同于物理地址),1为启用(此时线性地址通过分页机制转换成物理地址)
 * 实际上我进入的是平坦内存模式 flat memory mode
 */

	ljmp $(2<<3),$Protected32Code
/*VERY IMPORTANRT!code16->code32 ljmp for align*/

.code32
Protected32Code:
/*-----------To Do:flushpipeline------------*/
	movw $(1<<3),%ax
	movw %ax,%ds
	movw %ax,%es
	movw %ax,%gs
	movw %ax,%ss
//清空流水线
/*---load Udisk data(now in memory) to RAM address:0x100000--*/
//原理图类似赵炯《Linux内核完全注释修正版v3.0》
//图4.40内核代码在物理内存中的移动和分布情况
	movl $(BootGasStart+512),%esi
	movl $(HeadGasStart+512),%edi
//要加512，bootloader最初是 512Bytes
	xor %ecx,%ecx
	movb $16,%cl//我的128MBU盘CHS 16/255/63
	imul $(16*255*63/4) , %ecx //U盘总bit数除以4得到字节数
  subl  $(512/4),    %ecx //启动区大小
	movl $main,%esi
	movl $0x280000,%edi
//上面已经计算出ecx的大小
//mov $(88888),%ecx //88888
	call copy
	ljmp $(3<<3),$0x0000
//To C code seg,addr:0x280000:0000 0000
copy:
	movl (%esi),%eax
	addl $4,%esi
	movl %eax,(%edi)
	addl $4,%edi
	subl $1,%ecx
	jnz copy
	ret
/*Real mode 下的 global descriptor table
 *gdt最坑人的地方在于，intel为了兼容16位机和32位机,
 *把gdt的数拆开来放，非常碎，很难有直观感觉
 *gdt具体数位的摆放可参考《从实模式到保护模式》P201的图
 *，也可见赵炯《Linux内核完全注释修正版v3.0》P90分段机制的图
 */
temp_gdt:
	.word 0x0000,0x0000,0x0000,0x0000 //null segment
	.word 0xffff,0x0000,0x9200,0x00cf //data segment

	/*---code segment----------------*/

	.word 0xffff,0x0000,0x9a00,0x00c7 //gas code
	.word 0xffff,0x0000,0x9a28,0x00c7 //c code
	//0x9a=0x10011010:P(Segment present)1b DPL2b S:1b TYPE:4b
	//TYPE 101X  read,execute
	//见《从实模式到保护模式》P201的图
gdt_info:
	.word 31 /* 4*8-1 length0->31*/
	.long temp_gdt/*gdt address */
main:
//编译链接阶段main.c的主函数RiOSmain就要链接到此处来
/*Notes:
The 3 methods for enabling the A20 Gate :
1.Keyboard Controller
2.BIOS Function
3.System Port
	----http://kernelx.weebly.com/a20-address-line.html
*/
