/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月05日 星期三 16时36分34秒
 ************************************************************************/

#include<stdio.h>

#include"PLC_serial.h"
int main(void)
{
    int sun = 0;
    int fd;
    char *buf[10];
    char buff[10];
    fd = initPLC_serial("/dev/ttyO5",9600,7,2,1);
    while(1)
    {

        sun = 500;
        sprintf(buff,"%d",sun);
        buf[0] = buff;
        write_PLC_serial(fd,buf,1,450,60);
        
        while(1);

    }

    return 0;
}



