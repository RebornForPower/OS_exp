/*
 *  RiOS/bootloader.S
 *
 *  Copyright (C) 2017  Curie （邱日）
 */

.global  start
.code16

start:
/*-------------------------------fat12 file system head------------------------*/
	jmp entry
	/*U disk file format: fat12*/
	.byte 0x00
	.ascii "RiOS    "				/*8 Bytes*/
	.word 512 					/*sector*/
	.byte 1						/*cluster*/
	.word 1 					/*FAT start*/
	.byte 2     					/*FAT number*/
	.word 224   					/*root dir num*/
	.word 2880  					/*total:2880 sectors*/
	.byte 0xf0  					/*disk type*/
	.word 9     					/*FAT length:9 sectors*/
	.word 18    					/*a track has 18 sectors*/
	.word 2     					/*num of (magnetic) heads*/
	.long 0     					/*do not use partition*/
	.long 2880  					/*capacity of disk to rewrite*/
	.byte 0,0,0x29
	.long 0xffffffff 				/*label*/
	.ascii "RiOSudisk  "				/*11 Bytes*/
	.ascii "FAT12   "				/*8 Bytes*/
	.fill 18   		   		        /*18 byte free space*/
/*-------------------------------fat12 file system head end---------------------*/

entry:
	mov $0,%ax
	mov %ax,%ss
	mov %ax,%ds
	mov %ax,%es 					/*initialize*/
	mov $0x7c00,%sp
	mov %dl,(0x7dfe)				/*restore drive number*/

	mov $msg,%si
	call print					/*hello world!*/

	mov $0x800,%ax 					/*read u disk from sector2(Cylinder:0,Head:0,Sector:2)*/
	mov %ax,%es	  				/*memory address= 0x8200 chs=002*/
	mov $0x00,%ch 					/*cylinder 0*/
	mov $0x00,%dh 					/*head     0*/
	mov $0x01,%cl 					/*sector   2*/
	mov $0x00,%bx


readsectors:						//read 63 sectors (loop)
	xor %di,%di         				/*di :fail to read udisk for how many times*/
	call readudisk      				/*read 16 bits (2 bytes) at a time*/
	mov %es,%ax
	add $0x0020,%ax
	mov %ax,%es
	inc %cl
	cmp $63,%cl        	  			/*read sectors for 63 times(one at a time)*/
	jbe readsectors
							//read 63 sectors done
	mov $0x01,%cl   				/*sector start from 1*/
	inc %dh
	cmp 10,%dh					/*read heads >10 will get error*/
	jb readsectors

	mov $readUmsg,%si
	call print
	mov $successmsg,%si 				/*read u disk successfully*/
	call print
	mov $pleasewait3smsg,%si
	call print
	call Sleep3s


	jmp 0xc400					/*boot partition start address:0x8000+512=0x8200*/
	###jmp 到哪里和makefile里的这句	#cd kernel;make address=0xc400;mv kernel.out ../有关

dead:							/*loop forever*/
	jmp dead

Sleep3s:						//Sleep for about 3 s*/
	mov $0x5ff,%ax					/*depend on the speed of your PC,recommand value:0x5ff~0x5fff*/
rep2:
	mov $0x5fff,%cx
rep10ms:
	loop rep10ms
	dec %ax
	cmp $0x0000,%ax
	jne rep2
	ret


readudisk:            					//read contents of U disk to RAM*/
	mov (0x7dfe),%dl				/*restore drive number*/
	mov $0x02,%ah
							/*
							 *read floppy/hard disk in CHS mode
							 *  ---http://wiki.osdev.org/BIOS
							 */
	mov $0x01,%al 					/*number of udisk sectors to handle*/
	int $0x13 					/*INT 0x13 = mass storage (disk, floppy) access*/
	jc readUdiskFail
	jnc readok
	add $0x01,%di
	cmp $0x05,%di       				/*try for 5 times*/
	jae READUDISKFAIL     				/*fail for 5 times*/
	mov $0x00,%ah
	mov $0x00,%dl
	int $0x13
	jb readudisk


readok:							//readudisk
	ret

readUdiskFail:						//fail to read U disk
	mov $readFailmsg,%si
	call print					/*error msg*/
	hlt
	//ret
	jmp dead            				/*endless loop*/

READUDISKFAIL:
	mov $READFAILMSG,%si
	call print					/*error msg*/
	hlt
	//ret
	jmp dead           		 		/*endless loop*/



print :							//print func,usage:eg. mov $msg,%si;call print
	movb (%si),%al					/*note:movb*/
	add $1,%si
	cmp $0,%al
	je finish
	mov $0x0e,%ah
	mov $15,%bx					/*set color(do not work)*/
	int $0x10
	jmp print
finish:
							/*hlt*/
	ret

msg:
						.asciz "\r\nhello world!"
readFailmsg:
						.asciz "\r\nread U disk fail! "
READFAILMSG:
						.asciz "\r\nFAIL TO READ U DISK!!!"
successmsg:
						.asciz "\r\nread all sectors of U disk successfully. "
readsectormsg:
						.asciz "\r\nreading u disk ...."
pleasewait3smsg:
						.asciz "\r\nbootloader is working,please wait 3s......"
readUmsg:
						.asciz "\r\n   read 128MB Udisk to memory 0x8000"
.org 510
.word 0xaa55


/*Notes:
1.Ultraso :
	128MB U disk,C/H/S :16/255/63
2.boot from Udisk:
	$sudo qemu-system-x86_64   -drive file=/dev/sdb
3.fatal error:
	gnu/stubs-32.h: No such file or directory :dependency error.
	To solve this,$sudo apt-get install libc6-dev-i386
4.write to Udisk:
	$sudo dd if=kernel of=/dev/sdb
5.install wxhexeditor (Ubuntu)

6.My Udisk info: 128MB,C/H/S :16/255/63
  128MB RiOS Udisk,c*h*s=(cylinder+1)*(head+1)*sector
=(254+1)*(15+1)*63 sectors,1 sector = 512 bytes
volumn of U disk =(c*h*s)*512Bytes=131604480Bytes
	         =(131604480/1024)KB=128520KB=128MB
7.bootloader 512Bytes
8.如果0xaa55 ,不在第511字节和512字节，bios就会报错 No bootable device
9.我的128MB U盘,16个柱面，255个磁头，63个扇区(软碟通格式化时提示)
10.fat12能表示的最大空间只有4096*16K=64M。fat12一般建议簇大小是16K
FAT12文件系统能表示的最大簇数是2^12=4096
一般簇大小不超过32K。因此，fat12能表示的最大空间只有4096*16K=64M。
11.512=0x200 0x8000+512=0x8200
*/
