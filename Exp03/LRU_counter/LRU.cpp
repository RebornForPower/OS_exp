/*本程序实现了计数器方式的LRU*/
//要求： 模拟页面固定分配与局部替换策略,实现 LRU(2 种实现方法)、CLOCK(时钟页面
// 替换算法)共 2 种请求式页面替换算法,将页面、替换过程通过图形化方式可视化展现,计
// 算命中率。
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
typedef struct Page{
        int pageNum;
        int counter;
};
typedef Page QElemType;
#include "queue.h"
using namespace std;
#include "queue.c"
const int PHYSICAL_MEM_NUM = 4;
/*总共有4个物理存储器*/
bool Hit(LinkQueue *q,int PageNumValue){
        QueuePtr p;
        p=q->front->next;

        while(p){
                if(p->data.pageNum==PageNumValue){/*若命中*/
                        p->data.counter=-1;/*因为之后要统一加一，就会变成0*//*归0*/
                        return true;/*命中*/
                }
        p=p->next;
      }
      return false;/*未命中*/
}

int main()
{
         LinkQueue *q;
         q=(LinkQueue*)malloc(sizeof(LinkQueue));
         initQueue(q);
         int i;
         int visit[10]={1,2,3,1,4,5,1,2,1,4};
/*若命中，计数器清0
 *否则，检查是否满了
 *      若未满，直接添加
 *      若满了，替换
 */
         for(i=0;i<10;i++){
                if(Hit(q,visit[i]))  ;/*成功命中*/
                else{/*未命中*/
                        if(QueueLen(q)<PHYSICAL_MEM_NUM){
/*若没有命中，但没满，直接添加就可以了*/
                                Page newPage;
                                newPage.pageNum=visit[i];
                                newPage.counter=-1;/*因为之后要统一加一，就会变成0*/
                                EnQueue(q,newPage);
                        }
                        else{
/*若没有命中，而且存储器满了，就要替换*/
                                int maxcounter=getMaxcounter(q);
                                QueuePtr p;
                                p=q->front->next;
                                while(p){
                                        if(p->data.counter==maxcounter){
/*找到最大的进行替换,替换之后要把计数器清0*/
                                                p->data.pageNum=visit[i];
                                                p->data.counter=-1;/*因为之后要统一加一，就会变成0*/
                                        }
                                        p=p->next;
                                }

                        }
                }
                CounterIncrement(q);
                cout<<"\nPagequeue:   visit"<<visit[i]<<"\n";
                pFunction=&Operation_printcounter;traverse(q);
                cout<<endl;/*括号内的数是计数器的值*/
                //  queue_print(q);
         }


         cout<<"\nPagequeue:\n";
         pFunction=&Operation_print;/*指定全局函数指针，遍历要做的操作*/
         traverse(q);
         //queue_print(q);
        return 0;
}

