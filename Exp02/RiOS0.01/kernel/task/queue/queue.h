/*
 *  RiOS/task/queue/queue.h
 *
 *  Copyright (C) 2017  Curie （邱日）
 */

typedef task_struct QElemType;
/*typedef int QElemType;*/
typedef struct QNode
{
	QElemType data;
	struct QNode *next;
}QNode ,*QueuePtr;

typedef struct L
{
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

void initQueue (LinkQueue *q);
void EnQueue(LinkQueue *q,QElemType e);
void DeQueue(LinkQueue *q,QElemType *e);
void pushpQueue(LinkQueue *q,QueuePtr ptsk);
/*pushpQueue专为task_struct所打造，它不是push进一个元素，
 *而是push进一个已经malloc搞好的一坨节点，(不仅仅包括元素)
 *由于不需要再分配空间，因此函数也变得简单了
 */
void popQueue(LinkQueue *q);
/*popQueue和DeQueue功能一样，但不返回被删的东西*/
QueuePtr getPpopQueue(LinkQueue *q);
/*专业回收不要的指针
 *专为task_struct打造，pop出来的那个指针不是free掉，而是废物利用*/
void DetroyQueue(LinkQueue *q);
void Input(LinkQueue *q);
void PrintQueue(LinkQueue *q);
void GetTop(LinkQueue *q,QElemType *e);
QueuePtr getpTop(LinkQueue *q);
int  QueueLen(LinkQueue *q);

/*task_struct 相关*/
void print_taskQueue(LinkQueue *q);
void print_One_task(task_struct tsk);
