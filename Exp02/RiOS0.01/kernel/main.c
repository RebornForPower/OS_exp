/*
 *  RiOS/kernel/main.c
 *
 *  Copyright (C) 2017  Curie （邱日）
 */

#define   NULL      ((void   *)0)
/*注意（char *）0和（void *）0的区别，前者只是空指针值，后者可以转化为任意类型的指针*/
/*--------- 80*25 Text MODE------------*/
unsigned int jiffies=0;/*从开启8253计时器后的滴答数*/
unsigned char SCREEN_X=0;/*下一个显示字符的x位置*/
unsigned char SCREEN_Y=0;/*下一个显示字符的y位置*/
int RiOSmain();/*NOTE!! int RiOSmain():real main function*/
/*注意，由于链接的一些问题，在RiOSmain的定义之前只能放声明和.h文件
 *.c文件应当放在RiOSmain的定义之后，不然会出错
 */
#include "mm/memory.h" //unsigned int Ri_malloc(unsigned int size);
Mem_table * RiOSmm=(struct Mem_table *)0x3c0000;/*用于内存管理的全局结构体指针，在initialize里初始化*/
/*在initialize里面调用init_mem_table(RiOSmm);*/
#define Black_greenT 0b00000010
/*#define _setTcolor pVRAM[(80*SCREEN_Y+SCREEN_X)*2+1]=0b11000010*/
#define _setTcolor pVRAM[(80*SCREEN_Y+SCREEN_X)*2+1]=0b00000010
#include "console/console.h"
#include "include/asm/8259_8253.h"
#include "init/gdt_idt.h"
#include "init/initialize.h"
#include "init/interrupt/interrupt.h"
#include "task/task.h"
#include "task/queue/queue.h"
#include "include/rios/xwindow.h"
int currentTASKstate = 0;/*当前是否有任务，至关重要的全局变量*/
/*NOTE!!在RiOSmain函数的定义头上不能放.c文件，只能放.h头文件,不然
 *会报错“Trying to execute code outside RAM or ROM”*/
/*---在RiOSmain上面只能放函数的声明，不要把.c文件放在这里，不然出错----*/
LinkQueue *q1;
LinkQueue *q2;
LinkQueue *q3;
/*全局队列指针，MLFQ多级反馈队列，这里我们搞三级队列，这里是三个头
 *注意，一定要对这三个LinkQueue *进行malloc
 */
typedef QueuePtr tskStruQptr;
tskStruQptr current;
/*内核全局变量current当前任务指针
 *current->data类型为task_struct,这里current取从队列中摘下的一个节点
 */
void showtaskMSG();
int RiOSmain()
{//console main(NOTE!!real main function.defined in RiOS/kernel/kernel.ld)
                initialize();/*-init/initialize.c-*/
                console_cls_black();
                q1=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));
                initQueue(q1);/*千万不要忘记初始化*/
                q2=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));
                initQueue(q2);/*千万不要忘记初始化*/
                q3=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));
                initQueue(q3);/*千万不要忘记初始化*/
                current=(QueuePtr)Ri_malloc(sizeof(QNode));
                currentTASKstate=0;
                showtaskMSG();/*显示提示信息*/
                _8253timer_interrupt_enable();
/*三级反馈队列的初始化，初始化完了我再开时钟中断*/
/*在main函数里我好像一个任务都没有创建，其实我的任务创建放在中断中
 *任务创建请见RiOS/kernel/task/task.c的IntoReady()函数
 */
                while(1){;}//死循环
		return 0;
}
/*这里的.c文件不要放在RiOSmain函数定义上面，不然会出错*/
#include "mm/memory.c"
#include "console/console.c"
#include "init/8259_8253.c"
#include "init/gdt_idt.c"
#include "init/initialize.c"
#include "init/interrupt/interrupt.c"
#include "task/task.c"
#include "task/queue/queue.c"

/* unsigned int jiffies=0;*/
/*全局变量jiffies是从开机后的滴答数，非常重要我把定义放到开头*/
void _8253timer_interrupt(int *esp)
{
                #define _8259know 0x60
                outb(PIC0_OCW2	,_8259know);/*NOTE!!中断响应，这句话必须要有*/
                ++jiffies;
                xy_putdec(jiffies,0,0);
/*-------------------在时钟中断内的进程调度*----------------*/
                do_timer();/*定义在RiOS/kernel/task/task.h中，它do_timer调用schedule函数*/
                return;
}//~

void showtaskMSG()
{
                unsigned char *pVRAM=(char*)0xb8000;
                resetSCREENYX(0,10);
                char str0[]="----jiffies(System Time)-----\n";
                printlnColorful(str0);
                resetSCREENYX(1,0);
                char str1[]="Q1Len-----pid----start_time--run_time\n";
                printlnColorful(str1);
                resetSCREENYX(3,0);
                char str2[]="Q2Len-----pid----start_time--run_time\n";
                printlnColorful(str2);
                resetSCREENYX(5,0);
                char str3[]="Q3Len-----pid----start_time--run_time\n";
                printlnColorful(str3);

                resetSCREENYX(15,10);
                char str4[]="----MultiStageFeedbackQueue Process Schedule Algorithm-----\n";
                char str5[]="|   L1 Queue :Round Robin Algorithm                       |\n";
                char str6[]="|   L2 Queue :Round Robin Algorithm                       |\n";
                char str7[]="|   L3 Queue :First Come First Served                     |\n";
                printlnColorful(str4);
                resetSCREENYX(16,10);printlnColorful(str5);
                resetSCREENYX(17,10);printlnColorful(str6);
                resetSCREENYX(18,10);printlnColorful(str7);

                char infomsg[]="RiOS       Copyright (C) 2017  QiuRi \n";
                resetSCREENYX(23,10);printlnColorful(infomsg);

                resetSCREENYX(9,0);
                char str8[]="current task--pid--start_time--need--run_time\n";
                printlnColorful(str8);

                resetSCREENYX(1,55);
                char str9[]="task1\n";
                printlnColorful(str9);
                resetSCREENYX(2,55);
                char str10[]="task2\n";
                printlnColorful(str10);
                resetSCREENYX(3,55);
                char str11[]="task3\n";
                printlnColorful(str11);
                resetSCREENYX(4,55);
                char str12[]="task4\n";
                printlnColorful(str12);
                resetSCREENYX(5,55);
                char str13[]="task5\n";
                printlnColorful(str13);
                resetSCREENYX(6,55);
                char str14[]="task6\n";
                printlnColorful(str14);




                return;
}
/*---- xwindow 图形界面暂时不做，还是kernel最重要----*/
#include "lib/character/RiASCII.txt"
#include "lib/character/RiAlphaNum.txt"
#include "xwindow/xwindow.c"
