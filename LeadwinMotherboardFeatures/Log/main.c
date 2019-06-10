/*************************************************************************
	> File Name: main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年05月27日 星期一 10时37分24秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include"Log.h"

#define LOG "log.txt"

int main(void)
{
    /*初始化日志*/
    FILE * log = NULL;
    int i = 0;
    char data[100] = {0};
    printf("11111\n");
    log = InitLog(LOG);
    if(NULL == log)
    {
        printf("初始化错误!\n");
        return -1;
    }
    printf("初始化完成\n");
    while(1)
    {
        sprintf(data,"所属文件: %s,所属函数: %s,行号: %d,>>>>>>ID = %d\n",__FILE__,__FUNCTION__,__LINE__,i);
        WriteLog(data);
        printf("ID = %d\n",i);
        sleep(5);
        i++;
    }


    return 0;
}
