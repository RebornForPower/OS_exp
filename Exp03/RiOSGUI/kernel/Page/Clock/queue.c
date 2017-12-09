
void initQueue (LinkQueue *q)
{
                q->front=(QueuePtr)malloc(sizeof(QNode));
                q->rear=q->front;
                if(!q->front ) return;
                q->front ->next =NULL;
//  q->rear->next=NULL;
                return;
}

void EnQueue(LinkQueue *q,QElemType e)
{
                QueuePtr p;
                p=(QueuePtr)malloc(sizeof(QNode));
                if(!p)exit(1);/*以后要注释掉，改成其他处理*/
                p->data =e;
                p->next =NULL;
                q->rear ->next =p;
                q->rear =p;
                return;
}

void DeQueue(LinkQueue *q,QElemType *e)
{
                if(q->front == q->rear )return ;
                QueuePtr p;
                p=q->front ->next ;
                *e=p->data ;/*NOTE!!在主函数 int *tmpDel,DeQueue(&q,&tmpDel);putdec(tmpDel)*/
                //e=p->data;
                q->front ->next =p->next ;
                if(q->rear == p) q->rear = q->front ;
                /* free(p);*/
                free(p);
}



void GetTop(LinkQueue *q,QElemType *e)
{
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


void queue_print(LinkQueue *q){/*NOTE!!此函数的用于Debug,测试时拿int做,typedef int QElemType;*/
     QueuePtr p;
     p=q->front->next;
     while(p){
     printf("%d ",p->data.pageNum);
     /*这个只能用在data QElemType是int时，当类型变了要重写queue_print函数*/
     p=p->next;
   }
 }
