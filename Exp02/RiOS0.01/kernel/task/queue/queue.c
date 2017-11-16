/*
 *  RiOS/task/queue/queue.c
 *
 *  Copyright (C) 2017  Curie （邱日）
 */

 /*1.注意在第一次使用Ri_malloc之前先要对内存管理的部分init一下，
  *而且 NOTE!! 要把不是系统已经用了的地方给free掉
  *要free，不然全是0，没有可用空间
  *2.使用这些函数时要注意，要用取地址字符 & NOTE!! ，我这是纯C,不能用C++的引用
  */
/*NOTE!! q1=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));*/
/*NOTE!! q1=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));*/
/*NOTE!! q1=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));*/
void initQueue (LinkQueue *q)
{
/*usage:  LinkQueue *q;q=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));
initQueue(q);NOTE!! 先malloc再init*/
                q->front=(QueuePtr)Ri_malloc(sizeof(QNode));
                q->rear=q->front;
                if(!q->front ) return;
                q->front ->next =NULL;
//  q->rear->next=NULL;
                return;
}

void EnQueue(LinkQueue *q,QElemType e)
{
                QueuePtr p;
                p=(QueuePtr)Ri_malloc(sizeof(QNode));
                if(!p)putdec(404);/*以后要注释掉，改成其他处理*/
                p->data =e;
                p->next =NULL;
                q->rear ->next =p;
                q->rear =p;
                return;
}

void pushpQueue(LinkQueue *q,QueuePtr ptsk)
{
                q->rear ->next =ptsk;
                q->rear =ptsk;
                return;
}

void DeQueue(LinkQueue *q,QElemType *e)
{
/*usage:  QElemType e;e=(QElemType)Ri_malloc(sizeof(QElemType)); DeQueue(q,e);NOTE!! &*/
                if(q->front == q->rear )return ;
                QueuePtr p;
                p=q->front ->next ;
                *e=p->data ;/*NOTE!!在主函数 int *tmpDel,DeQueue(&q,&tmpDel);putdec(tmpDel)*/
                //e=p->data;
                q->front ->next =p->next ;
                if(q->rear == p) q->rear = q->front ;
                /* free(p);*/
                Ri_free((void *)p,sizeof(p)),p=NULL;
}

void popQueue(LinkQueue *q)
{
/*usage:  QElemType e;e=(QElemType)Ri_malloc(sizeof(QElemType)); DeQueue(q,e);NOTE!! &*/
                if(q->front == q->rear )return ;
                QueuePtr p;
                p=q->front ->next ;
//*e=p->data ;/*NOTE!!在主函数 int *tmpDel,DeQueue(&q,&tmpDel);putdec(tmpDel)*/
                q->front ->next =p->next ;
                if(q->rear == p) q->rear = q->front ;
/* free(p);*/
                Ri_free((void *)p,sizeof(p)),p=NULL;
}

QueuePtr getPpopQueue(LinkQueue *q)
{
                if(q->front == q->rear )return NULL;
                QueuePtr p;
                p=q->front ->next ;
//*e=p->data ;/*NOTE!!在主函数 int *tmpDel,DeQueue(&q,&tmpDel);putdec(tmpDel)*/
                q->front ->next =p->next ;
                if(q->rear == p) q->rear = q->front ;
/* free(p);*/
/*Ri_free((void *)p,sizeof(p)),p=NULL;*/
                p->next=NULL;
                return p;
/*getPpopQueue与popQueue的区别就在于，pop出来的那个指针不是free掉
 *而是当个宝捡起来，把next置为空，p本身留着，准备插在其他队列的屁股后面
 *此函数专为task_struct打造
 */
}

void DetroyQueue(LinkQueue *q)
{/*usage：  DetroyQueue(q);NOTE!! & */
                while(q->front){
                        q->rear =q->front ->next ;
/* free(q->front );*/
                        Ri_free(q->front,sizeof(q->front)),q->front=NULL;
                        q->front =q->rear ;
                }
                return;
}


void GetTop(LinkQueue *q,QElemType *e)
{/*usage: QElemType *temp;temp=(QElemType)Ri_malloc(sizeof(QElemType));GetTop(q,temp);注意要加取地址符号*/
                QueuePtr p=q->front->next  ;
                *e=p ->data ;
                return;
}

QueuePtr getpTop(LinkQueue *q){
  return (q->front->next);
}


int  QueueLen(LinkQueue *q){
    int i=0;
    QueuePtr p;
    p=q->front->next;
    while(p){
      i++;
      p=p->next;
    }
    return i;
}
/*注意在第一次使用Ri_malloc之前先要对内存管理的部分init一下，
 *而且 NOTE!! 要把不是系统已经用了的地方给free掉
 *要free，不然全是0，没有可用空间
 */
/*unsigned int Ri_malloc(unsigned int size){
 * // if(RiOSmm_initialized)memman_init(RiOSmm),RiOSmm_initialized=1;
 * return memman_alloc(RiOSmm,size);
 *}
 *void Ri_free(unsigned int upointer,int size){
 *memman_free(RiOSmm,upointer,size);
 *}
 */
 /*NOTE!!注意我这是裸机上的纯C，不能用C++的引用*/
/*NOTE!!NOTE!!initQueue,千万不要忘记，用队列要先初始化！！
 *先调用initQueue,再去入队出队
 */
















 // void queue_print(LinkQueue *q){/*NOTE!!此函数的用于Debug,测试时拿int做,typedef int QElemType;*/
 //     QueuePtr p;
 //     p=q->front->next;
 //     while(p){
 //     putdec(p->data),tab();
 //     /*这个只能用在data QElemType是int时，当类型变了要重写queue_print函数*/
 //     p=p->next;
 //   }
 // }
// void test_queue(){/*NOTE!!此函数的用于Debug,测试时拿int做,typedef int QElemType;*/
//      LinkQueue *q;
// /*NOTE!!q1=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));
//  *NOTE!!特别注意,LinkQueue q1;q1本身也要malloc，结构体指针自己也要初始化
//  */
//      q=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));/*Very important!*/
// /*队列指针本身也要初始化，malloc,不malloc的话
//  *一个队列各种操作没有问题，两个也行，当我搞到第八个队列时就完蛋了
//  *LinkQueue *指针也要初始化
//  */
//      //q=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));/*Very important!*/
//      //q=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));/*Very important!*/
//      initQueue(q);
//      QElemType data1=2;
//      QElemType data2=3;
//      int i;
//      QElemType *tmpDel;
//      QElemType *tmpTop;
//      tmpDel=(QElemType)Ri_malloc(sizeof(QElemType));
//      tmpTop=(QElemType)Ri_malloc(sizeof(QElemType));
//      for(i=0;i<20;i++){
//        EnQueue(q,2*i+1);
//      }
//      queue_print(q),endl();
//      DeQueue(q,tmpDel);
//      queue_print(q),endl();
//      char str1[]="Del:\n";
//      printnoln(str1);putdec(*tmpDel),tab();
//      char str2[]="GetTop:\n";
//      printnoln(str2),GetTop(q,tmpTop),putdec(*tmpTop),tab();
//      char str3[]="Len:\n";
//      printnoln(str3),putdec(QueueLen(q)),tab();
//      return;
// }
// void _int_test3queues(){/*NOTE!!此函数的用于Debug,测试时拿int做,typedef int QElemType;*/
//     LinkQueue *q1;
//     q1=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));
//     initQueue(q1);
//     LinkQueue *q2;
//     q2=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));
//     initQueue(q2);
//     LinkQueue *q3;
//     q3=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));
//     initQueue(q3);
//     int i;
//     for(i=1;i<=3;i++)
//     EnQueue(q1,i);
//     for(i=21;i<26;i++)
//     EnQueue(q2,i);
//     for(i=31;i<39;i++)
//     EnQueue(q3,i);
//     queue_print(q1),endl();
//     queue_print(q2),endl();
//     queue_print(q3),endl();
// }
// LinkQueue * _MultiLevelIntQueue(){
//   LinkQueue *q1;
//   q1=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));
//   initQueue(q1);
//   LinkQueue *q2;
//   q2=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));
//   initQueue(q2);
//   LinkQueue *q3;
//   q3=(LinkQueue*)Ri_malloc(sizeof(LinkQueue));
//   initQueue(q3);
//   int i;
//   for(i=1;i<=3;i++)
//   EnQueue(q1,i);
//   for(i=21;i<26;i++)
//   EnQueue(q2,i);
//   for(i=31;i<39;i++)
//   EnQueue(q3,i);
//   char str1[]="Initial:\n";endl(),printnoln(str1),endl();
//   queue_print(q1),endl();queue_print(q2),endl();queue_print(q3),endl();
//
//   QElemType *tmpTop;
//   QElemType *tmpDel;
//   while(QueueLen(q1)!=0){
//     GetTop(q1,tmpTop);DeQueue(q1,tmpDel);
//     EnQueue(q2,*tmpTop);
//   }
//   char str2[]="Then:\n";endl(),printnoln(str2),endl();
//   queue_print(q1),endl();queue_print(q2),endl();queue_print(q3),endl();
//
//   while (QueueLen(q2)!=0) {
//     GetTop(q2,tmpTop);DeQueue(q2,tmpDel);
//     EnQueue(q3,*tmpTop);
//   }
//   char str3[]="After:\n";endl(),printnoln(str3),endl();
//   queue_print(q1),endl();queue_print(q2),endl();queue_print(q3),endl();
//
//   while (QueueLen(q3)!=0) {
//     GetTop(q3,tmpTop);DeQueue(q3,tmpDel);
//   }
//   char str4[]="End:\n";endl(),printnoln(str4),endl();
//   queue_print(q1),endl();queue_print(q2),endl();queue_print(q3),endl();
//   char str5[]="Now,queue3 Len:\n";
//   printnoln(str5),putdec(QueueLen(q3)),tab();
//
//
// };
