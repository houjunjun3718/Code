/*************************************************************************
	> File Name: main_Retriever.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年08月29日 星期三 17时05分45秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>


#include"Retriever.h"

int main(void)
{
    int fd;
    int ren;
    Retriever_pup data[16];
    fd = initRetriever("/dev/ttyUSB0",9600,8,0,1);
    printf("main 20\n");
    while(1)
    {
        ren = Retriever_data_processing_function(fd,data,1);
        printf("ren = %d\n",ren);
        printf("time[%s]\n",data[0].time);
        printf("pup_A_temp[%s]\n",data[0].pup_A_temp);
        printf("pup_B_temp[%s]\n",data[0].pup_B_temp);
    }
    closeRetriever(fd);
    return 0;
}

