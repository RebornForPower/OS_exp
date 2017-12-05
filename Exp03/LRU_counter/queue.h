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
/*实验二*/
void Operation_print(QElemType E){
	printf(" %d ",E.pageNum);
}
void Operation_printcounter(QElemType E){
	printf(" Page:%d (%d) ",E.pageNum,E.counter);
}
void  CounterIncrement(LinkQueue *q){
	QueuePtr p;
	p=q->front->next;

	while(p){
	p->data.counter++;/*每个计数器都加一*/
	p=p->next;
      }
}
void (*pFunction)(QElemType);/*全局函数指针*/
void  traverse(LinkQueue *q){
	QueuePtr p;
	p=q->front->next;

	while(p){
	(*pFunction)(p->data);
/*在使用本函数之前，先设置一下全局函数指针pFunction*/
	//Operation(p->data);
	p=p->next;
      }
}
int getMaxcounter(LinkQueue *q){
	int maxcounter=-1;
	QueuePtr p;
	p=q->front->next;

	while(p){
	if(p->data.counter>maxcounter)
		maxcounter=p->data.counter;
	p=p->next;
      }
      return maxcounter;
}
/*实验二*/
void initQueue (LinkQueue *q);
void EnQueue(LinkQueue *q,QElemType e);
void DeQueue(LinkQueue *q,QElemType *e);

void DetroyQueue(LinkQueue *q);
void Input(LinkQueue *q);
void GetTop(LinkQueue *q,QElemType *e);
