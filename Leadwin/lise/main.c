/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月19日 星期三 09时57分08秒
 ************************************************************************/

#include<stdio.h>

#include"lise.h"

int main(void)
{
    char *val = NULL;
    Queue queue;
    InitQueue(&queue);
    IsEmptyQueue(&queue);
    InsertQueue(&queue,"houjunjun");
    InsertQueue(&queue,"liboshi");
    InsertQueue(&queue,"guoqiang");
    InsertQueue(&queue,"lishiling");
    IsEmptyQueue(&queue); //判断是否为空
    TraverseQueue(&queue); //遍历数据
    val = DeleteQueue(&queue);//出队数据
//    printf("121212121212\n");
    printf("%s\n",val);//出队数据
    val = DeleteQueue(&queue);//出队数据
//    printf("121212121212\n");
    printf("%s\n",val);//出队数据
//    printf("1212121212\n");
    TraverseQueue(&queue);//遍历数据
    printf("%d\n",LengthQueue(&queue));//求队列长度
    ClearQueue(&queue);//清空队列
    IsEmptyQueue(&queue);//判断是否为空
    DestroyQueue(&queue);//摧毁队列

    return 0;
}


