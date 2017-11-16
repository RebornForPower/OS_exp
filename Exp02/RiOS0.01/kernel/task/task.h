/*
 *  RiOS/task/task.h
 *
 *  Copyright (C) 2017  Curie （邱日）
 */


 /*本系统采用多级反馈队列进程调度算法
  *,本系统中的MLFQ算法对其他调度算法进行了综合，
  *即我采用三级队列，上层队列(前两层)采用时间片轮转
  *最下层(第三层)队列采用先来先服务
  */
#define L1_timepiece 200
#define L2_timepiece 400
/*#define L3_timepiece 800*/
/*其实在本系统中，不用再定义第三级的时间片了,
 *最下层队列我采用先来先服务，反正一直做，直到该任务完成
 */

/*进程的三态模型,对应task_struct中的proc_state*/
#define TASK_RUNNING    1
/*running 运行态*/
#define TASK_READY      2
/*ready 就绪态*/
#define TASK_BLOCKED    3
/*blocked 阻塞态 即睡眠态 sleep*/

/*三级反馈队列，每个队列最多容纳的任务*/
#define MAX_TASK_L1 3
#define MAX_TASK_L2 6
/*#define MAX_TASK_L3 512*/

//tss结构由硬件决定。
//赵炯《Linux0.11内核完全注释v3.0》P127图4-24 32位任务段tss格式
typedef struct tss32_struct{//tss结构由硬件决定，固定死了是104个字节。
  	long back_link; /* 16 high bits zero*/
  	long esp0;
  	long ss0;				/* 16 high bits zero*/
  	long esp1;
  	long ss1;				/* 16 high bits zero*/
  	long esp2;
  	long ss2;				/* 16 high bits zero*/
  	long cr3;

/* 32位寄存器 */
  	long eip;
  	long eflags;
  	long eax;
  	long ecx;
  	long edx;
  	long ebx;
  	long esp;
  	long ebp;
  	long esi;
  	long edi;

 /* 16位寄存器 */
  	long es;
  	long cs;
  	long ss;
  	long ds;
  	long fs;
  	long gs;

 /*----任务设置部分，任务切换时不会被写入CPU,但不能随意赋值，否则无法正常切换----*/
  	long ldt;
  	long trace_bitmap;// I/o位图基地址
}tss32_struct;//~

typedef struct task_struct{
  long proc_id;/*进程编号*/
  long start_time;/*进程创建时间*/
  long proc_state;/*进程状态*/
  long run_time;/*进程运行时间*/

  long priority;/*优先级*/
  long timepiece_needed;/*本进程运行完需要的时间片*/

  /*下面和task_struct关系不大，只是为了本次实验演示需要*/
  int proc_screen_x;
  int proc_screen_y;

  tss32_struct proc_tss32;
/*本来应当包括tss32_struct*/
}task_struct;

task_struct NULLTASK={
  .proc_id=0,/*进程编号*/
  .start_time=0,/*进程创建时间*/
  .proc_state=0,/*进程状态*/
  .run_time=0,/*进程运行时间*/

  .priority=0,/*优先级*/
  .timepiece_needed=0,/*本进程运行完需要的时间片*/

  /*下面和task_struct关系不大，只是为了本次实验演示需要*/
  .proc_screen_x=30,
  .proc_screen_y=30
};
/*全局指针current的data要为空的时候，我又不想把current变为NULL
 *那样current可能会有malloc方面的问题，我就搞一个这样的NULLTASK
 */

void do_timer();
/*do_timer是调用频率非常高的函数，在时钟中断中调用，do_timer
 *自己调用schedule
 */
 /*由于需要“手动创建进程，需要事先做好pcb”*/
task_struct* mkPCB(long priority,long start_time,\
  long timepiece_needed,\
  int proc_screen_x,int proc_screen_y);
/*pid的生成我采用通过时间jiffies获取伪随机数的方法*/

void IntoReady();
void resetcurrent();
void show_current();
long mkpid();
/*下面定义一些进程的原语*/
/*进程原语句主要有：进程的创建、终止、阻塞、唤醒和切换*/
void Create_task(task_struct *mtsk);
/*在本系统中，先在mkPCB中将task_struct
 *空间分配好，值都设好，返回给Create_task一个指针
 */
void Destroy_current_task();
/*进程撤销原语*/
void run_task(task_struct mtsk);

/*show_task仅仅为展示效果*/
void reflashX_2_4_6();
void show_task_queues();
void show_queueL1();
void show_queueL2();
void show_queueL3();
/*show_task这些没有什么技术含量*/


void sleep_on(task_struct *mtsk_struct);
void wakeup(task_struct *mtsk_struct);
void schedule();/*进程调度，将在时钟中断中调用*/
void MLFQ();/*Multi-Level Feedback Queue*/
/*多级反馈队列调度算法,在本系统中，多级反馈综合了时间片轮转
 *和先来先服务算法,上层用时间片轮转，下层用先来先服务
 */
void RR_L1();
void RR_L2();
void RR_L1L2();/*Round Robin*/
/*时间片轮转算法,在本系统中，第一级第二级队列用时间片轮转*/
void FCFS_L3();/*First Come First Served*/
/*先来先服务算法,在本系统中，最后一级第三级用先来先服务算法*/


/*tss32_struct 32位保护模式，long 32位，4字节
 *tss32_struct 共 26个数据项，每项4字节，共104字节
 *tss由硬件决定，固定死了是104字节，故它的范围是0～103
 */
 /*struct 的声明 注意在C和C++里不同
  *在C中定义一个结构体类型要用typedef:
  *C中若光有 struct 甲，那么在别处用甲还是报错：unknown type name 甲
  *所以要用 typedef 来定义这个类型,来取个别名
  *若没有typedef ,其他地方申明变量时 struct student qiuri;//Struct 不能丢
  *若有typedef struct Student{...}Stu; 可以 Stu qiuri;//省略struct
  *而C++若写 struct Student{}; ，在别处是认的,直接 Student qiuri;（C++可以省略struct ,但c语言不可以）
  */
 //struct tss32_struct;//NOTE!! 注意在用此结构体定义新的结构体时应该：struct tss32_struct tss32_mystruct;
