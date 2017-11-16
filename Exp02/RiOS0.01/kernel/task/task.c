/*
 *  RiOS/task/task.c
 *
 *  Copyright (C) 2017  Curie （邱日）
 */
/*多级反馈队列调度算法，第一第二级用时间片轮转，最后一级用先来先服务*/
 void do_timer()
 {
                Ri_cli();/*关中断*/
                if(currentTASKstate==1){//如果已经有了任务，当前全局任务指针有所指向
                        if(jiffies>=current->data.start_time)current->data.run_time++;
                }
                if(currentTASKstate==1)run_task(current->data);
                schedule();/*调度函数，在RiOS/kernel/task/task.h中*/
                show_task_queues();
                IntoReady();
                Ri_sti();/*开中断*/
 }//~

void IntoReady()
{
/*在任务PCB的start_time时刻创建该任务，jiffies注意要和PCB的start_time两者一致
 *mkPCB最后两个参数是在屏幕的显示位置
 */
                if(jiffies==200)Create_task(mkPCB(1,200,888,     1,60));
/*优先级第1级，200时间开始，需要时间片888*/
                if(jiffies==300)Create_task(mkPCB(3,300,777,     2,60));
/*优先级第3级，300时间开始，需要时间片777*/
                if(jiffies==600)Create_task(mkPCB(1,600,40,      3,60));
/*优先级第1级，600时间开始，需要时间片40*/
                if(jiffies==1000)Create_task(mkPCB(1,1000,222,   4,60));
/*优先级第1级，1000时间开始，需要时间片222*/
                if(jiffies==1200)Create_task(mkPCB(1,1200,999,   5,60));
/*优先级第1级，1200时间开始，需要时间片999*/
                if(jiffies==1400)Create_task(mkPCB(2,1400,666,   6,60));
/*优先级第2级，1400时间开始，需要时间片666*/

}//~

void run_task(task_struct mtsk)
{
/*这里仅为了演示*/
//if(currentTASKstate==1)xy_putdec(mtsk.run_time,mtsk.proc_screen_x,mtsk.proc_screen_y);
 xy_putdec(mtsk.run_time,mtsk.proc_screen_x,mtsk.proc_screen_y);
}//~

void schedule()
{/*进程调度，将在时钟中断中调用*/
                MLFQ();/*多级反馈队列调度算法*/
}//~

void MLFQ()
{
/*Multi-Level Feedback Queue
 *多级反馈队列调度算法
 *前两级采用时间片轮转，最后一级采用先来先服务
 */
                if(QueueLen(q1)>0|QueueLen(q2)>0)RR_L1L2();
                FCFS_L3();
 }//~

void RR_L1()
{/*对第一级采用时间片轮转*/
                if(currentTASKstate==0&&QueueLen(q1)!=0){
                        current=getPpopQueue(q1);
                        currentTASKstate=1;
                }
                if(currentTASKstate==1){
                        if(current->data.run_time>=current->data.timepiece_needed){
/*任务运行完毕*/
                        Destroy_current_task();
                        resetcurrent();
                        currentTASKstate=0;
                        return;
                        }
                }
                if(currentTASKstate==1){
                        if(current->data.priority==1&&current->data.run_time>=L1_timepiece){
                                if(QueueLen(q2)<MAX_TASK_L2){
                                        current->data.priority=2;
                                        pushpQueue(q2,current);
                                        resetcurrent();
                                }
                                else{
                                        current->data.priority=3;
                                        pushpQueue(q3,current);
                                        resetcurrent();
                                }
                        }
                }
}//~

void RR_L2()
{/*对第二级采用时间片轮转*/
                if(currentTASKstate==0&&QueueLen(q2)!=0){
                        current=getPpopQueue(q2);
                        currentTASKstate=1;
                }
                if(currentTASKstate==1){
                        if(current->data.run_time>=current->data.timepiece_needed){
/*任务运行完毕*/
                                Destroy_current_task();
                                resetcurrent();
                                currentTASKstate=0;
                                return;
                        }
                }
                if(currentTASKstate==1){
                        if(current->data.priority==2&&current->data.run_time>=L2_timepiece){
                                current->data.priority=3;
                                pushpQueue(q3,current);
                                resetcurrent();
                        }
                }
}//~

void RR_L1L2()
{/*Round Robin*/
/*对第一第二级队列用时间片轮转算法*/
    RR_L1();
    RR_L2();
}

void FCFS_L3()
{/*First Come First Served*/
/*对第三级队列用先来先服务调度算法*/
                if(currentTASKstate==0&&QueueLen(q3)!=0){
                        current=getPpopQueue(q3);
                        currentTASKstate=1;
                }
                if(currentTASKstate==1){
                        if(current->data.run_time<current->data.timepiece_needed){
                                return;
                        }
                        else {
//  currentTASKstate=0;
//  resetcurrent();//current->data=NULLTASK;
                        Destroy_current_task();
                        resetcurrent();
                        currentTASKstate=0;
                        return;
                        }
                }
                if(QueueLen(q1)==0&&QueueLen(q2)==0&&QueueLen(q3)==0)return;
 }

long mkpid()
{/*pid的生成我采用通过时间jiffies获取伪随机数的方法*/
                if(jiffies==0)return 1;/*防止pid返回0*/
                return((int)jiffies<<16|jiffies<<8);
}

task_struct* mkPCB(long priority,long start_time, \
 long timepiece_needed, \
 int proc_screen_x,int proc_screen_y )
 {
/*pid的生成我采用通过时间jiffies获取伪随机数的方法*/
                task_struct *pnewPCB;
                pnewPCB=(task_struct*)Ri_malloc(sizeof(task_struct));
/*必须要分配空间*/
                pnewPCB->proc_id=mkpid();
                pnewPCB->start_time=start_time;
                pnewPCB->proc_state=TASK_READY;
/*创建完就默认ready,这个暂时不要外界赋值*/
                pnewPCB->run_time=0;
/*新进程尚未运行*/
                pnewPCB->priority=priority;
                pnewPCB->timepiece_needed=timepiece_needed;
                pnewPCB->proc_screen_x=proc_screen_x;
                pnewPCB->proc_screen_y=proc_screen_y;
                return pnewPCB;
 }

void resetcurrent()
{
                current=(QueuePtr)Ri_malloc(sizeof(QNode));
                current->data=NULLTASK;
                return;
}

void Create_task(task_struct *mtsk)
{/*进程创建原语*/
                if(mtsk){
                        if((mtsk->priority==1)&&(QueueLen(q1)<MAX_TASK_L1))EnQueue(q1,*mtsk);
                        else if((mtsk->priority==2)&&(QueueLen(q2)<MAX_TASK_L2))EnQueue(q2,*mtsk);
                        else if(mtsk->priority==3)EnQueue(q3,*mtsk);
/*入相应的队，注意EnQueue(LinkQueue*q,QElemType e);*/
                        }
                return;
}
void Destroy_current_task()
{/*进程撤销原语*/
                Ri_free((void *)current,sizeof(QueuePtr)),current=NULL;
                return;
}//~

void show_current()
{
        unsigned char *pVRAM=(char*)0xb8000;
        resetSCREENYX(10,0);
        //char str0[]="-----------------------------------------------------\n";
        char str0[]="                                                     \n";
        println(str0);
/*每次显示前需要刷新一下，以免有上次画面的残留*/
                if(currentTASKstate==1){
                        xy_putdec(current->data.proc_id,10,10);
                        xy_putdec(current->data.start_time,10,20);
                        xy_putdec(current->data.timepiece_needed,10,30);
                        xy_putdec(current->data.run_time,10,40);
                }
                return;
}//~

void reflashX_2_4_6()
{/*刷新屏幕的指定行*/
                unsigned char *pVRAM=(char*)0xb8000;
                resetSCREENYX(2,0);
                //char str0[]="-------------------------------------\n";
                char str0[]="                                     \n";
                println(str0);
                resetSCREENYX(4,0);
                println(str0);
                resetSCREENYX(6,0);
                println(str0);
}

void show_task_queues()
{
                reflashX_2_4_6();/*把要显示的这几行刷新，省得有屏幕残留*/
                show_queueL1();
                show_queueL2();
                show_queueL3();
                show_current();
                //if(currentTASKstate==1)xy_putdec(q3->front->next->data.start_time,20,20);
}//~

void show_queueL1()
{
/*先展示队列长度*/
                xy_putdec(QueueLen(q1),2,1);//第一级队列的队列长度
/*再展示队头的pid*/
                if(QueueLen(q1)>0){
                        xy_putdec(q1->front->next->data.proc_id,2,10);
/*如果有队头，展示队头的开始时间*/
                        if(QueueLen(q1)>0)xy_putdec(q1->front->next->data.start_time,2,20);
                        else xy_putdec(0,2,20);
/*展现队头的运行时间*/
                        xy_putdec(q1->front->next->data.run_time,2,30);
                }
                else {/*如果队列里面没有东西，显示也没有意义了*/
                        xy_putdec(0,2,10),xy_putdec(0,2,20),xy_putdec(0,2,30);
                }
}//~

void show_queueL2()
{
/*先展示队列长度*/
                xy_putdec(QueueLen(q2),4,1);//第二级队列的队列长度
/*再展示队头的pid*/
                if(QueueLen(q2)>0){
                        xy_putdec(q2->front->next->data.proc_id,4,10);
/*如果有队头，展示队头的开始时间*/
                        if(QueueLen(q2)>0)xy_putdec(q2->front->next->data.start_time,4,20);
                        else xy_putdec(0,4,20);
                        xy_putdec(q2->front->next->data.run_time,4,30);
                }
                else {/*如果队列里面没有东西，显示也没有意义了*/
                        xy_putdec(0,4,10),xy_putdec(0,4,20),xy_putdec(0,4,30);
                }
}//~

void show_queueL3()
{
/*先展示队列长度*/
                xy_putdec(QueueLen(q3),6,1);//第一级队列的队列长度
/*再展示队头的pid*/
                if(QueueLen(q3)>0){
                        xy_putdec(q3->front->next->data.proc_id,6,10);
/*如果有队头，展示队头的开始时间*/
                        if(QueueLen(q3)>0)xy_putdec(q3->front->next->data.start_time,6,20);
                        else xy_putdec(0,6,20);
                        xy_putdec(q3->front->next->data.run_time,6,30);
                }
                else {/*如果队列里面没有东西，显示也没有意义了*/
                        xy_putdec(0,6,10),xy_putdec(0,6,20),xy_putdec(0,6,30);
                }
}
