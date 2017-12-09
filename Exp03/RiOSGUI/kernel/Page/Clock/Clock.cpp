// 模拟页面固定分配与局部替换策略,实现 LRU(2 种实现方法)、CLOCK(时钟页面
// 替换算法)共 2 种请求式页面替换算法,将页面、替换过程通过图形化方式可视化展现,计
// 算命中率。
/*Clock policy*/
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
typedef struct Page{
        int pageNum;/*页面号*/
        int flag;/*use_bit 引用位 clock算法里用到*/
        int counter;/*计数器 LRU 计数方法实现中用到*/
};
typedef Page QElemType;

const int PHYSICAL_MEM_NUM = 4;
/*请求页面序列号(访问串),长度为 10。*/
int ClockPageseq[10][PHYSICAL_MEM_NUM]={-1};
int onetofour=0;
int requesttime=0;
#include "queue.h"
using namespace std;
#include "queue.c"
/*总共有4个物理存储器*/
/*Clock Policy:NOTE!! important
 * page first loaded: flag<=1
 * page fault, referenced :flag<=1
 * To replace a page: scan, find a page which flag=0,(if flag=1,reset to 0)
 *   if it exists,replace it,
 *   else set all 1flag to 0,stop at original position,replace that page
 */
void roundclockVisit(LinkQueue *q,int visit_i,QueuePtr &phistory)
{/*This is GOOD CODE*/
        QueuePtr p;
        p=phistory;
        int i;
        int hit=0;
/*To judge hit or not */
        for(i=0;i<PHYSICAL_MEM_NUM;i++){
                if(p->data.pageNum==visit_i){
                        hit=1;
/*hit,do not replace*/
                        p->data.flag=1;
/*ok, work done .This time we DONOT have to replace*/
                        return;
                }
                p=p->next;
        }
/*When it comes to replace a page*/
        p=phistory;
        int replace=0;
        for(i=0;i<PHYSICAL_MEM_NUM;i++){
/* A full cycle */
                if(p->data.flag==0){
/* replace it */
                        p->data.pageNum=visit_i;
                        p->data.flag=1;
                        replace=1;
                        break;
                }else{
                        p->data.flag=0;
                }
                p=p->next;
        }
        if(!replace){
/*Scan no found ,replace orginal position page*/
                p->data.pageNum=visit_i;
                p->data.flag=1;
                replace=1;
        }
	phistory=p->next;
	printf("\n");
}

void  roundqueue_traverse(LinkQueue *q)
{
	QueuePtr p;
        p=q->front->next;
        int i;
	for(i=0;i<PHYSICAL_MEM_NUM;i++){
	       (*pFunction)(p->data);
/*在使用本函数之前，先设置一下全局函数指针pFunction*/
	        p=p->next;
        }
}

void showClockpageseq(){
	//cout<<"int Clock_Pageseq[10][4]={";
	//不知道为什么这里写40就不行（裸机上没有图形显示），写48或许有点冗余，就行
	cout<<"int Clock_Pageseq[48]={";
	int i,j;
	for(i=0;i<10;i++){
		for(j=0;j<4;j++){
			printf(" %d ",ClockPageseq[i][j]);
			if(!(i==9&&j==3))cout<<",";
		}
		cout<<endl;
	}
	cout<<"};";
}

int main()
{
	printf("/*");
        LinkQueue *q;
        q=(LinkQueue*)malloc(sizeof(LinkQueue));
        initQueue(q);
        int i;
        /*int visit[10]={1,2,3,1,4,5,1,2,1,4};*/
	#include "LRU_counter.txt"
/*三个算法中，只由LRU计数器方法的来输入一次，之后的按这个输入的来，不需要再输入了*/
	
        for(i=0;i<PHYSICAL_MEM_NUM;i++){
/*此为Clock算法，物理存储器有4个，先弄好*/
                Page newPage;
                newPage.pageNum=-1;/*无效*/
                newPage.flag=-1;/*无效*/
                EnQueue(q,newPage);
        }
/*整个队已经弄好，现在让它头尾相接*/
        q->rear->next=q->front->next;/*屁股接到头上，循环起来*/
        QueuePtr phistory;
        phistory=q->front->next;
        for(i=0;i<10;i++){
                 roundclockVisit(q,visit[i],phistory);
                 cout<<"\nPage_roundqueue:   visit"<<visit[i]<<"\n";
                 //cout<<"\nPage_roundqueue:   visit"<<visit[i]<<phistory->data.pageNum<<"\n";
                 pFunction=&Operation_printflag;roundqueue_traverse(q);
			requesttime++;
        }
	printf("*/ \n");

	showClockpageseq();
        return 0;
}
