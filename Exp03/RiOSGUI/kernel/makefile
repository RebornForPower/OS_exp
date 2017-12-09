laji=head.asm head.elf head.o  main.elf \
main.o main.out \
main_gas.o \
main.asm mainPlus.o
###CFLAGS = -g #gdb 加载符号表
ok:
	gcc -nostdinc -I.  -fno-stack-protector -c RiOShead.S -m32 -o head.o
	ld -m elf_i386 head.o -e start -Ttext 0xc400 -o head.elf
	objcopy -S -O binary -j .text head.elf  head.out
	objdump -S head.elf > head.asm
	###################gcc  -fno-stack-protector -c main.c -m32 -o main.o

############################################################尝试引入C++
	gcc  -fno-stack-protector -c main.c -m32 -o main.o

###########################################################尝试引入C++
	#g++  -fno-stack-protector -c  main.cpp -m32 -w -fpermissive  -o main.o
	############################################
	gcc -c -m32 main_gas.S -o main_gas.o
	ld -m elf_i386 -r -o mainPlus.o main.o main_gas.o
	##ld -m elf_i386  -e RiOSmain -T kernel.ld -o main.elf main.o
	ld -m elf_i386  -e RiOSmain -T kernel.ld -o main.elf mainPlus.o
	############################################
	objdump -S -D main.elf >main.asm
	objcopy  -S -O binary  main.elf main.out
	cat main.out >> head.out
	mv head.out  kernel.out
	rm -f $(laji)
clean:
	rm -f $(laji)
#Notes:
#gcc  -nostdinc -I.  -fno-stack-protector -c main.c -m32 -o main.o
#-nostdinc  不在标准系统目录中搜索头文件，只在-I指定的目录中搜索
#不加这句 会编译stdio.h啥的，速度会变慢很多，但不加这句 stdio.h time.h没法用
#g++  -nostdinc -I.  -fpermissive -fno-stack-protector -c main.cpp -m32 -o main.o
#warning: invalid conversion from ‘int’ to ‘unsigned char*’ [-fpermissive]
#-fpermissive:error -> warning
#make -s (slient,禁止命令的显示)
# main.c 需要用到 汇编，因此写了main_gas.S，通过C里的exter外部函数定义在汇编语言中
#先把.c和.S都编译成.o再链接，链接成mainPlus.o
#在gcc编译main_gas.S时要加-c选项,只编译，不链接;还需要-m elf_i386 选项，不然i386和x86_64不兼容
#ld -m elf_i386 -r -o mainPlus.o main.o maingas.o
#这里必须要加 -r 选项（raw）(直接链接多个.o文件的方法)
