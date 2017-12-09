/*
 *  RiOS/kernel/main.c
 *
 *  Copyright (C) 2017  Curie （邱日）
 */
unsigned char *pVRAM=(char*)0xa0000;
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
// #include "console/console.h"
#include "include/asm/8259_8253.h"
#include "init/gdt_idt.h"
#include "init/initialize.h"
#include "init/interrupt/interrupt.h"
// #include "task/task.h"
// #include "task/queue/queue.h"
#include "include/rios/xwindow.h"
void Sleep(int secs);
// char Rialphabet[832];
// char Rinumber[288];

void PageReplacement(int *mypage);
void DrawPages(int mypage[4]);
void DrawPages_visit(int mypage[4],int visit_value,int replace);
int pagesPosx=0;
int pagesPosy=0;
int drawLinecount=0;

int getdiff(int A[4],int B[4]);
void drawhitrate(int hittimes);
void showalpha(int method);
int RiOSmain()
{//xwindow main
        initialize();
        _8253timer_interrupt_enable();
/*允许时钟中断*/
int i,j;
//int mypage[4]={5,1,2,4};/*这个初始值没有用，之后会赋值的*/
int mypage[4]={0,0,0,0};
showalpha(1);/*第一种方法： LRU计数器*/
/*这是LRU算法的计数器实现*/
#include "Page/LRU_counter/LRU_counter.txt"
// int A[48]={ 1 , 0 , 0 , 0,
// 1 , 2 , 0 , 0,
// 1 , 2 , 3 , 0 ,
// 1 , 2 , 3 , 0,
//
// 1 , 2 , 3 , 4 ,
// 1 , 5 , 3 , 4 ,
// 1 , 5 , 3 , 4 ,
// 1 , 5 , 2 , 4,
//
// 1 , 5 , 2 , 4 ,
// 1 , 5 , 2 , 4
// };
// /*不能声明LRU[40]否则全是0*/

int LRU_counter_replace=0;/*每次替换的是哪一个*/
int beforemypage[4]={0,0,0,0};
int hittimes=0;
float hitrate;
        for(i=0;i<10;i++){
                for(j=0;j<4;j++){beforemypage[j]=mypage[j];}
                for(j=0;j<4;j++){
                        mypage[j]=LRU_counter_Pageseq[i*4+j];

                }
                DrawPages_visit(mypage,visit[i],getdiff(mypage,beforemypage));
                //if(A[i]!=0)DrawPages(mypage);
                if(getdiff(mypage,beforemypage)==0)hittimes++;
        }
        hitrate=hittimes*1.0/(1.0*10);
        drawhitrate(hittimes);


        Sleep(3);
        cls();

showalpha(2);/*第2种方法： CLOCK算法*/
/*这是Clock算法*/
#include "Page/Clock/Clock.txt"
/*Clock_Pageseq[48];*/
for(i=0;i<4;i++)mypage[i]=0;
for(i=0;i<4;i++)beforemypage[i]=0;
hittimes=0;
drawLinecount=0;pagesPosx=0;pagesPosy=0;/*画图相关*/
for(i=0;i<10;i++){
        for(j=0;j<4;j++){beforemypage[j]=mypage[j];}
        for(j=0;j<4;j++){
                mypage[j]=Clock_Pageseq[i*4+j];

        }
        DrawPages_visit(mypage,visit[i],getdiff(mypage,beforemypage));
        //if(A[i]!=0)DrawPages(mypage);
        if(getdiff(mypage,beforemypage)==0)hittimes++;
}
hitrate=hittimes*1.0/(1.0*10);
drawhitrate(hittimes);
Sleep(3);
cls();


showalpha(3);/*第3种方法： LRU算法的栈实现*/
/*这是LRU算法的栈实现*/
#include "Page/LRU_stack/LRU_stack.txt"
/*int LRUstackPageseq[48]*/
for(i=0;i<4;i++)mypage[i]=0;
for(i=0;i<4;i++)beforemypage[i]=0;
hittimes=0;
drawLinecount=0;pagesPosx=0;pagesPosy=0;/*画图相关*/
for(i=0;i<10;i++){
        for(j=0;j<4;j++){beforemypage[j]=mypage[j];}
        for(j=0;j<4;j++){
                mypage[j]=LRUstackPageseq[i*4+j];

        }
        DrawPages_visit(mypage,visit[i],getdiff(mypage,beforemypage));
        //if(A[i]!=0)DrawPages(mypage);
        if(getdiff(mypage,beforemypage)==0)hittimes++;
}
hitrate=hittimes*1.0/(1.0*10);
drawhitrate(hittimes);


        while(1){;}//死循环
}
int getdiff(int now[4],int before[4]){
        int i,j;
        int flag=0;
        for(i=0;i<4;i++){
                flag=0;
                for(j=0;j<4;j++){
                        if(now[i]==before[j]){
                                flag=1;
                        }
                }
                if(!flag)return now[i];
        }
        //&&before[i]!=0
        return 0;
}
void drawhitrate(int hittimes){
        #include "xwindow/NUM.c"
        // int ge=((int)(hitrate*100))%10;
        // int shi=(((int)hitrate*100)-ge)%10;
        //  Draw_en(Ri_NUM+shi*16,250,150);
        char percent[16]={0x00, 0x00, 0x00, 0x44, 0xa4, 0xa8, 0xa8, 0xa8, 0x54, 0x1a, 0x2a, 0x2a, 0x2a, 0x44, 0x00, 0x00};
         Draw_en(percent,250-8,150);
         Draw_en(Ri_NUM+hittimes*16,250,150);
         Draw_en(Ri_NUM+0*16,258,150);
         return;
}
void DrawPages(int mypage[4])
{
        int i;
        #include "xwindow/NUM.c"
        for(i=0;i<4;i++){
         Pagerectangle(DARK_GRAY,pagesPosx,pagesPosy+20*i,pagesPosx+30,pagesPosy+20*(i+1));
         Draw_en(Ri_NUM+mypage[i]*16,pagesPosx+0,pagesPosy+20*i);
        //  Sleep(1);
        }
        pagesPosx+=50;
        int temp=drawLinecount+1;
        drawLinecount=(drawLinecount+1)%12;
        if(drawLinecount>=6){
                pagesPosy=100;
                if(drawLinecount==6)pagesPosx=0;
        }else{
                pagesPosy=0;
        }
        Sleep(1);
        if(temp==12){
                cls();
                pagesPosx=0;
                pagesPosy=0;
        }
}
void DrawPages_visit(int mypage[4],int visit_value,int replace)
{
        int i;
        #include "xwindow/NUM.c"
        for(i=0;i<4;i++){
         if(mypage[i]!=visit_value)Pagerectangle(DARK_ORANGE,pagesPosx,pagesPosy+20*i,pagesPosx+30,pagesPosy+20*(i+1));
         else{
                 Pagerectangle(DARK_BLUE,pagesPosx,pagesPosy+20*i,pagesPosx+30,pagesPosy+20*(i+1));
         }
         Draw_en(Ri_NUM+mypage[i]*16,pagesPosx+0,pagesPosy+20*i);
        //  Sleep(1);
        }
        if(replace!=0) Draw_en(Ri_NUM+replace*16,pagesPosx+0,pagesPosy+20*4);
        pagesPosx+=50;
        int temp=drawLinecount+1;
        drawLinecount=(drawLinecount+1)%12;
        if(drawLinecount>=6){
                pagesPosy=100;
                if(drawLinecount==6)pagesPosx=0;
        }else{
                pagesPosy=0;
        }
        Sleep(1);
        if(temp==12){
                cls();
                pagesPosx=0;
                pagesPosy=0;
        }
}
void PageReplacement(int *mypage)
{
int i=0,j=0;

        //for(i=0;i<10;i++){
                //  for(j=0;j<4;j++)mypage[j]=LRU_counter_Pageseq[4*i+j];
                DrawPages(mypage);
        //}
}
void showalpha(int method){
        #include "xwindow/RiAlphaNum.txt"
        if(method==1){
                Draw_en(Rialphabet+('L'-'A')*16,200,150-16);
                Draw_en(Rialphabet+('R'-'A')*16,200+8,150-16);
                Draw_en(Rialphabet+('U'-'A')*16,200+8*2,150-16);
                Draw_en(Rialphabet+('C'-'A')*16,200+8*4,150-16);
                Draw_en(Rialphabet+('O'-'A')*16,200+8*5,150-16);
                Draw_en(Rialphabet+('U'-'A')*16,200+8*6,150-16);
                Draw_en(Rialphabet+('N'-'A')*16,200+8*7,150-16);
                Draw_en(Rialphabet+('T'-'A')*16,200+8*8,150-16);
                Draw_en(Rialphabet+('E'-'A')*16,200+8*9,150-16);
                Draw_en(Rialphabet+('R'-'A')*16,200+8*10,150-16);
        }else if(method==2){
                Draw_en(Rialphabet+('C'-'A')*16,200,150-16);
                Draw_en(Rialphabet+('L'-'A')*16,200+8,150-16);
                Draw_en(Rialphabet+('O'-'A')*16,200+8*2,150-16);
                Draw_en(Rialphabet+('C'-'A')*16,200+8*3,150-16);
                Draw_en(Rialphabet+('K'-'A')*16,200+8*4,150-16);
        }else if(method==3){
                Draw_en(Rialphabet+('L'-'A')*16,200,150-16);
                Draw_en(Rialphabet+('R'-'A')*16,200+8,150-16);
                Draw_en(Rialphabet+('U'-'A')*16,200+8*2,150-16);
                Draw_en(Rialphabet+('S'-'A')*16,200+8*4,150-16);
                Draw_en(Rialphabet+('T'-'A')*16,200+8*5,150-16);
                Draw_en(Rialphabet+('A'-'A')*16,200+8*6,150-16);
                Draw_en(Rialphabet+('C'-'A')*16,200+8*7,150-16);
                Draw_en(Rialphabet+('K'-'A')*16,200+8*8,150-16);
        }

}
// void PageReplacement(int *mypage)
// {
//         int i=0;
//
//         #include "xwindow/NUM.c"
//         for(i=0;i<4;i++){
//          Pagerectangle(LIGHT_GREEN,0,20*i,30,20*(i+1));
//          Draw_en(Ri_NUM+mypage[i]*16,0,20*i);
//          Sleep(1);
//         }
//         for(i=1;i<=4;i++){
//         rectangle(WHITE,0,20*(i-1),30,20*i);
//         Pagerectangle(DARK_BLUE,0,20*(i-1),30,20*i);
//         Draw_en(Ri_NUM+(4-i)*16,0,20*(i-1));
//         Sleep(1);
//         }
//
// }
void Sleep(int secs)
{
              int i=0,j=0;
              for(;i<secs;i++)for(j=0;j<5000000;j++);
}//~
#include "lib/character/RiASCII.txt"
#include "lib/character/RiAlphaNum.txt"
/*这里的.c文件不要放在RiOSmain函数定义上面，不然会出错*/
#include "mm/memory.c"
// #include "console/console.c"
#include "init/8259_8253.c"
#include "init/gdt_idt.c"
#include "init/initialize.c"
#include "init/interrupt/interrupt.c"
// #include "task/task.c"
// #include "task/queue/queue.c"

void _8253timer_interrupt(int *esp)
{
                #define _8259know 0x60
                outb(PIC0_OCW2	,_8259know);/*NOTE!!中断响应，这句话必须要有*/
                ++jiffies;
                return;
}//~

/*---- xwindow 图形界面----*/
#include "xwindow/xwindow.c"
