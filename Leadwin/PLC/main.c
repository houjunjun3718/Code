/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年08月17日 星期五 09时35分33秒
 ************************************************************************/

#include<stdio.h>

#include"PLC_serial.h"
#define PLC_TTYUSB0 "/dev/ttyUSB0"

int main(void)
{
    int fd;
    int len;
    fd = initPLC_serial(PLC_TTYUSB0,9600,7,2,1);
    if(fd < 0)
    {
        perror("initPLC_serial:");
    }
    char *buf[10] = {"032.5"};
    char **buff;

    len = write_PLC_serial(fd,buf,1,451,60);
    if(len == 1)
    {
        printf("数据传输成功\n");
    }else{
        printf("数据传输失败\n");
        return -1;
    }

    len = read_PLC_serial(fd,buff,1,341,60);
    return 0;

}
