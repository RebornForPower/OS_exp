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
#include "queue.h"
using namespace std;
#include "queue.c"
const int PHYSICAL_MEM_NUM = 4;
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

int main()
{
        LinkQueue *q;
        q=(LinkQueue*)malloc(sizeof(LinkQueue));
        initQueue(q);
        int i;
        int visit[10]={1,2,3,1,4,5,1,2,1,4};
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
        }
        return 0;
}




































//
// #include <iostream>
// #include <vector>
// #include <queue>
// using namespace std;
// class Page{
// private:
//         int pageNum;
//         int counter;
// public:
//         Page(int pageNum){
//                 this->pageNum=pageNum;
//                 this->counter=0;
//         };
//         int getpagenum(){return pageNum;}
//         int getcounter(){return counter;}
//         friend bool operator <(Page pa,Page pb){
//                 //return pa.getcounter()<pb.getcounter();
//                 return pa.getpagenum()<pb.getpagenum();
//         }
//         void operator ++() {
//                 this.counter++;/*计数*/
//         }
// };
// void print_prio_queue(priority_queue <Page> PageQueue){
//         Page Temp(0);
//         while(!PageQueue.empty()){
//                 PageQueue.pop();
//                 Temp = PageQueue.top();
//                 cout<<Temp.getpagenum()<<" ";
//         }
// }
// int main(){
//         //priority_queue<Page, vector<Page>, cmptime> PageQueue;
//         priority_queue <Page> PageQueue;
//         int i;
//         int visit[10]={1,2,3,1,4,5,1,2,1,4};
//         int times=0;
//         for(i=0;i<10;i++,times++){
//
//                 cout<<visit[i]<<" ";
//                 PageQueue.push(Page(visit[i]));
//
//
//
//
//         }
//         cout<<"hello\n";
//         // Page Temp(0);
//         //
//         // while(!PageQueue.empty()){
//         //         // Temp=PageQueue.top();
//         //         // PageQueue.pop();
//         //         // cout<<Temp.getpagenum()<<" ";
//         //         PageQueue.pop();
//         //         Temp =PageQueue.top();
//         //          cout<<Temp.getpagenum()<<" ";
//         // }
//
//
//
//
// }
// int tmp[100];
// struct cmp1
// {
//      bool operator ()(int x, int y)
//     {
//         return x > y;//小的优先级高
//     }
// };
// struct cmp2
// {
//     bool operator ()(const int x, const int y)
//     {
//         return tmp[x] > tmp[y];
//         //tmp[]小的优先级高，由于可以在队外改变队内的值，
//         //所以使用此方法达不到真正的优先，建议用结构体类型。
//     }
// };
// struct node
// {
//     int x, y;
//     friend bool operator < (node a, node b)
//     {
//         return a.x > b.x;//结构体中，x小的优先级高
//     }
// };
//
// priority_queue<int>q1;
// priority_queue<int, vector<int>, cmp1>q2;
// priority_queue<int, vector<int>, cmp2>q3;
// priority_queue<node>q4;
// int main()
// {
//     int i,j,k,m,n;
//     int x,y;
//     node a;
//     while(cin>>n)
//     {
//         for(int i=0;i<n;i++)
//         {
//             cin>>a.y>>a.x;
//             q4.push(a);
//         }
//         cout << endl;
//         while(!q4.empty())
//         {
//             cout<<q4.top().y <<" "<<q4.top().x<<endl;
//             q4.pop();
//         }
//         cout << endl;
//     }
//     return 0;
// }
