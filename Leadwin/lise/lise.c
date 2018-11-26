/*************************************************************************
	> File Name: lise.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月18日 星期二 09时26分31秒
    > 功能：使用单列表实现栈，进行数据的短期存储
 ************************************************************************/

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>


#include"lise.h"

/*************************************************************************
 *创建队列函数
 *参数１：队列头
 *返回值：无返回值
 * **********************************************************************/
void InitQueue(PQueue queue)
{
    queue->Front = queue->Rear = (PNODE)malloc(sizeof(NODE));  //动态开辟头结点，使队头，队尾指向该节点
    if(queue->Front == NULL)
    {
        printf("创建队列，分配内存失败．．．\n");
        exit(-1);
    }
    queue->Front->Next = NULL;
    printf("创建队列成功\n");
}

/*************************************************************************
 *判断队列是否为空函数
 *参数１：队列头
 *返回值：队列为空返回0，否则返回1;
 * **********************************************************************/
int IsEmptyQueue(PQueue queue)
{
    if(queue->Front == queue->Rear){
        printf("队列为空\n");
        return 0;
    }else{
        printf("队列不为空\n");
        return 1;
    }
}

/*************************************************************************
 *入队函数
 *参数１：队头
 *参数２：入队的数据
 *返回值：无返回值
 * **********************************************************************/
void InsertQueue(PQueue queue,char *data)
{
    PNODE P = (PNODE)malloc(sizeof(NODE));
    if(P == NULL){
        printf("内存分配失败，无法插入数据%s...\n",data);
        exit(1);
    }
    P->buff = data;
    P->Next = NULL;
    queue->Rear->Next = P;   //将原先的队尾节点指向新节点
    queue->Rear = P;         //更新队尾指针，使其指向新的节点
    printf("插入数据成功%s...\n",data);
}

/*************************************************************************
 *出队函数
 *参数１：队头
 *参数２：出队的数据
 *返回值：无返回值
 * **********************************************************************/
char *DeleteQueue(PQueue queue)
{
    char *data;
    if(IsEmptyQueue(queue) == 0){
        printf("队列以空，无法出队．．．\n");
        exit(-1);
    }
    PNODE P = queue->Front->Next;  //临时指针
    data = P->buff;        
    queue->Front->Next = P->Next;  //更新头节点
    if(queue->Rear == P){
        queue->Rear = queue->Front;
    }
    free(P);
    P = NULL;
    printf("出栈成功，出栈数据%s...\n",data);
    return data;
}

/*************************************************************************
 *摧毁队列函数
 *参数１：对头
 *返回值：无返回值
 * **********************************************************************/
void DestroyQueue(PQueue queue)
{
    while(queue->Front != NULL)
    {
        queue->Rear = queue->Front->Next;  //将尾指针指向头节点的下一个节点
        free(queue->Front);//free首节点
        queue->Front = queue->Rear;//将头指针指向尾指针
    }
    printf("队列摧毁成功...\n");
}

/*************************************************************************
 *遍历队列函数
 *参数１：队头
 *返回值：无返回值
 * **********************************************************************/
void TraverseQueue(PQueue queue)
{
//    printf("11111111111\n");
    if(IsEmptyQueue(queue) == 0)
    {
        exit(-1);
    }
//    printf("***********\n");
    PNODE P = queue->Front->Next;
    printf("开始遍历队列\n");
    while(P != NULL)
    {
        printf("%s\n",P->buff);
        P = P->Next;
    }
}

/*************************************************************************
 *清空队列函数
 *参数１：队头
 *返回值：无返回值
 * **********************************************************************/
void ClearQueue(PQueue queue)
{
    PNODE P = queue->Front->Next;
    PNODE Q = NULL;
    queue->Rear = queue->Front;
    queue->Front->Next = NULL;
    while(P != NULL)
    {
        Q = P;
        P = P->Next;
        free(Q);
    }
    printf("清空队列成功...\n");
}

/*************************************************************************
 *求队列长度函数
 *参数１：对头
 *返回值：返回队列的长度
 * **********************************************************************/
int LengthQueue(PQueue queue)
{
    int i = 0;
    if(IsEmptyQueue(queue) == 0)
    {
        exit(-1);
    }
    PNODE P = queue->Front->Next;
    printf("开始遍历队列\n");
    while(P != NULL)
    {
        i++;
        P = P->Next;
    }
    return i;
}





