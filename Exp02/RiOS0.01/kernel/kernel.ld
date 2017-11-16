/*
 *  RiOS/kernel/kernel.ld
 *
 *  Copyright (C) 2017  Curie （邱日）
 */
/*syntax learned from XV6 and JOS.
 *link script for RiOS/main.c
 *    reference:  https://github.com/leenjewel/xv6_learn/blob/learn/kernel.ld
 */
/*
 *这是一个链接脚本,它指明了RiOS/kernel/main.c的main函数是int RiOSmain()
 *在makefile里面非常重要
 */
OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(RiOSmain)/*main function:RiOSmain*/

SECTIONS
{
	/* Link the kernel at this address: "." means the current address */
	. = 0x0000;
	.text :  {
		*(.text)
	}
	.rodata : {
		*(.rodata)
	}

	.data : {
		*(.data)
	}

	.bss : {
		*(.bss)
	}


}
