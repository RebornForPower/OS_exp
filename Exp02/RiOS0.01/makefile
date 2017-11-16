
 #  RiOS/makefile
 #
 #  Copyright (C) 2017  Curie （邱日）

laji=bootloader.asm  bootloader.o \
bootloader.elf
laji2=bootloader.out kernel.out
ok:
	gcc -c bootloader.S -m32 -o bootloader.o
	ld -m elf_i386 bootloader.o -e start -Ttext 0x7c00 -o bootloader.elf
	objcopy -S -O binary -j .text bootloader.elf bootloader.out
	objdump -S bootloader.elf > bootloader.asm
	@dd if=/dev/zero of=bootloader.out seek=2 count=8888 >> /dev/zero
	cd kernel;make address=0xc400;mv kernel.out ../
	#cd kernel;make address=0x10000;mv kernel.out ../
	sudo mount -o loop   bootloader.out  /mnt
	sudo cp kernel.out /mnt
	sudo umount /mnt
	rm -f $(laji)
	cat bootloader.out>RiOS.img
	#qemu-system-x86_64   -m 888 -drive file=bootloader.out -monitor stdio
	rm -f $(laji2)

	qemu-system-x86_64   -m 666 -drive file=RiOS.img -monitor stdio
 	#qemu-system-x86_64   -m 666 -drive file=RiOS.img -monitor stdio -full-screen


#666MB
#标准输入输出来开启调试模式
#info registers查看寄存器的值
write2Udisk:
	#sudo dd if=bootloader.out of=/dev/sdb
	sudo dd if=RiOS.img of=/dev/sdb
bootfromUdisk:
	sudo qemu-system-x86_64   -drive file=/dev/sdb
######################################################
gdbport1234:
	sudo qemu-system-x86_64 -drive file=bootloader.out  -monitor stdio -gdb tcp::1234  -S

##如何用gdb和qemu调试:https://www.cnblogs.com/chineseboy/p/4216521.html
##在一个终端$ qemu-system-x86_64 -drive file=bootloader.out -gdb tcp::1234  -S
##指定端口1234 -S 选项stop，让qemu为我暂停 ，让我有时间打开另一个终端去用gdb来调试它
##然后再打开另一个终端 $ gdb  $ target remote :1234 $ c		(表示continue)
##直接打开qemu自带调试： qemu-system-x86_64加上选项-monitor stdio从标准输入输出获取控制信息
