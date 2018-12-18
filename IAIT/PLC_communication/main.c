/*************************************************************************
	> File Name: main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2018年12月05日 星期三 19时15分32秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include"PLC_communication.h"

#define PLC "/dev/ttyO5"

int main(void)
{
    int PLC_fd;
    PLC_fd = PLC_communication_init(PLC,9600,7,2,1);

    char *data[20] = {0};
    
    data[0] = "10180016";
    data[1] = "12-12 12:12";
    data[2] = "123";
    data[3] = "123";
    data[4] = "123";
    data[5] = "123";
    data[6] = "123";
    data[7] = "123";


    printf("%s\n",data[1]);

    printf("121212121212\n");
    if(0 >PLC_communication_write(PLC_fd,data,6,401,60))
    {
        printf("1cuowu!!!!!!!\n");
        return -1;
    }
    printf("11111111111\n");
/*
    if(0 >PLC_communication_write(PLC_fd,data,6,407,60))
    {
        printf("2cuowu!!!!!!!\n");
        return -1;
    }
    */
    printf("发送成功!!!!!\n");
    return 0;
}



