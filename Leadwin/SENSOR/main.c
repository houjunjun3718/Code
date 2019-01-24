/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年08月20日 星期一 14时49分07秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

#include"watchdog_2475.h"

int main(void)
{
    int fd;
    int ren;
    spectrum data;
    fd = initSpectrum("/dev/ttyUSB0",9600,8,0,1);
    printf("fd = %d\n",fd);
    while(1)
    {
        ren = read_Spectrum(fd,&data);
        printf("ren = %d\n",ren);
        printf("**************************************************\n");
        printf("%s\n",data.time);
        printf("%s\n",data.battery);
        printf("%s\n",data.PA);
        printf("%s\n",data.PB);
        printf("%s\n",data.PC);
        printf("%s\n",data.PD);
        printf("%s\n",data.PE);
        printf("%s\n",data.PF);
        printf("%s\n",data.PG);
        printf("%s\n",data.PH);
        printf("%s\n",data.PI);
        printf("%s\n",data.PJ);
        printf("%s\n",data.PK);
        printf("%s\n",data.PL);
        printf("**************************************************\n");  
        sleep(30);
    }
    cloceSpectrum(fd);
    return 0;
}

