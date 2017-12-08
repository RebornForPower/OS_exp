
#include <iostream>
#include "stdio.h"
#include "stdlib.h"
using namespace std;
const int PHYSICAL_MEM_NUM = 4;
/*物理存储器有四个*/
const int STACK_INIT_SIZE = PHYSICAL_MEM_NUM;
#define STACKINCREMENT 10
/*在本实验中并不需要。栈最多是PHYSICAL_MEM_NUM*/
typedef struct Page{
        int pageNum;
}Page;
typedef  Page ElemType;
typedef struct{
    ElemType *base;
    ElemType *top;
    int stacksize;
}sqStack;

/*LRU policy (stack implement)
 *LRU的栈实现算法描述可见《Operationg System Concepts》P335
 * page referenced :removed from stack,put on the top
 * stack top: most recently used
 */
 void initStack(sqStack &s)
 {
     s.base = (ElemType *)new ElemType[STACK_INIT_SIZE] ;
     if(!s.base) exit(0);
     s.top = s.base;
     s.stacksize = STACK_INIT_SIZE;
 }

 void Push(sqStack &s, ElemType e)
 {
     if(s.top - s.base >= s.stacksize)
 	{
 		s.base = (ElemType *)realloc(s.base, (s.stacksize + STACKINCREMENT)*sizeof(ElemType));
 		if(!s.base) exit(0);
 		s.top = s.base + s.stacksize;
 		s.stacksize = s.stacksize + STACKINCREMENT;
     }
 		*(s.top) = e;
 		s.top++;
 }

 void Pop(sqStack &s , ElemType &e)
 {
     if(s.top == s.base) return;
     e = *--(s.top);
 }

 int StackLen(sqStack s)
 {
     return (s.top - s.base) ;
 }

 void PrintStack(sqStack s)
 {
        for(int i=0;i<s.top-s.base;i++){
                printf(" Page:%d [ ] ",s.base[i].pageNum);
        }
 }

 int StackEmpty(sqStack s)
 {
 	if(s.base -s.top )
 		return 0;
 	return 1;
 }

 ElemType GetTop(sqStack s,ElemType &e)
 {

 	if(s.base ==s.top ){
                Page noPage;
                noPage.pageNum=-1;
                return  noPage;
        }
 	e=*(s.top -1);
 	return e;
 }

 void Visit(ElemType &e)
 {
        printf(" Page:%d [ ] ",e.pageNum);
 }

 void StackTraverse(sqStack s)
 {
 	for(int i=0;i<s.top -s.base ;i++)
 		Visit(s.base [i]);
 	cout<<endl;
 }
 int main()
 {
         int i;
         int visit[10]={1,2,3,1,4,5,1,2,1,4};
         sqStack LRU_Pagestack;
         sqStack LRU_PagestackHelper;/*辅助栈*/
         initStack(LRU_Pagestack);
         initStack(LRU_PagestackHelper);
         Page tempPage;
         tempPage.pageNum=-1;
         Page Pageout;
         Pageout.pageNum=-1;
         for(i=0;i<10;i++)
         {
/*一个一个翻出来找，找到最后一个若还没有碰到，
 *栈未满：最不经常使用的不用换，直接恢复现场，新来的放在顶上
 *栈已满：把这个最不经常使用的扔掉。把现场恢复，腾出来顶上空位给新来的
 */
                Page newPage;
                newPage.pageNum=visit[i];
/*LRU stack实现*/
                while(!StackEmpty(LRU_Pagestack)){
                        Pop(LRU_Pagestack,tempPage);
                        if(tempPage.pageNum==visit[i]){
/*若找到跳出循环*/                                break;
                        }
                        Push(LRU_PagestackHelper,tempPage);
/*一个一个地找*/
                }
/*若找到了*/
                if(!StackEmpty(LRU_Pagestack)){
                        Page HandPage=tempPage;
                        while(!StackEmpty(LRU_PagestackHelper)){
                                Pop(LRU_PagestackHelper,tempPage);
                                Push(LRU_Pagestack,tempPage);
                        }
                        Push(LRU_Pagestack,HandPage);
                }else{
                        if(StackLen(LRU_PagestackHelper)==PHYSICAL_MEM_NUM){
                                Pop(LRU_PagestackHelper,Pageout);
/*若满了，在辅助栈中把最老的淘汰*/
                        }
                        while(!StackEmpty(LRU_PagestackHelper)){
                                Pop(LRU_PagestackHelper,tempPage);
                                Push(LRU_Pagestack,tempPage);
                        }
/*若栈当时没满，元素放栈顶*/
                        if(StackLen(LRU_Pagestack)<PHYSICAL_MEM_NUM){
                                Push(LRU_Pagestack,newPage);
                        }
/*若栈当时满了删除栈底(之前在辅助栈中已经删除)，把新Page放栈顶*/
                }
                  cout<<"\nPagequeue:   visit"<<visit[i]<<"\n";
                   PrintStack(LRU_Pagestack);
        }
        return 0;
 }
