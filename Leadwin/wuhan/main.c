/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年08月20日 星期一 17时40分27秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#include"PLC_serial.h"

#include"watchdog_2475.h"

int main(void)
{
    int PLC_fd,watchdog_fd;
    spectrum data;
    int ren;
    int sun = 0;
    int sum = 0;
    char *buf[10];
    char buff_PA[10];
    char buff_PB[10];
    char buff_PC[10];
    char buff_PD[10];
    watchdog_fd = initSpectrum("/dev/ttyO2",9600,8,0,1); //初始化气象站的串口
    if(watchdog_fd < 0)
    {
        perror("initSpectrum:");
    }
    PLC_fd = initPLC_serial("/dev/ttyO5",9600,7,2,1);//初始化ＰＬＣ的串口
    if(PLC_fd < 0)
    {
        perror("initPLC_serial:");
    }
    while(1)
    {
        ren = read_Spectrum(watchdog_fd,&data);
//        printf("ren = %d 0标示成功 -1表示失败\n",ren); 
        sun = atof(data.PB)*10;
        sprintf(buff_PB,"%d",sun);
        buf[0] = buff_PB;
        printf("温度:%s\n",buf[0]);
        sun = atof(data.PA)*10;
        sprintf(buff_PA,"%d",sun);
        buf[1] = buff_PA;
        printf("湿度:%s\n",buf[1]);
        sun = atof(data.PC)*100;
        sprintf(buff_PC,"%d",sun);
        buf[2] = buff_PC;
        printf("ＥＣ:%s\n",buf[2]);
        sum = (sum+1)%10;
        sprintf(buff_PD,"%d",sum);
        buf[3] = buff_PD;
        printf("计数:%s\n",buf[3]);
//        memcpy(buf[0],data.PA,10);
//        sprintf(buf[0],"%s",data.PA);
//        printf("%s\n",data.PA);
        /*
        printf("温度:%s\n",buf[0]);
        printf("湿度:%s\n",buf[1]);
        printf("ＥＣ:%s\n",buf[2]);
        */
        ren = write_PLC_serial(PLC_fd,buf,4,550,60);
        
        while(4 != ren)
        {
            sleep(1);
            ren = write_PLC_serial(PLC_fd,buf,4,550,60);
        }
        
        printf("ren = %d 1表示成功　其他表示失败\n",ren);
        ren = write_PLC_serial(PLC_fd,buf,4,450,60);
        
        while(4 != ren)
        {
            sleep(1);
            ren = write_PLC_serial(PLC_fd,buf,4,450,60);
        }
        printf("ren = %d 1表示成功　其他表示失败\n",ren);
        ren = write_PLC_serial(PLC_fd,buf,4,510,60);
        
        while(4 != ren)
        {
            sleep(1);
            ren = write_PLC_serial(PLC_fd,buf,4,510,60);
        }
        printf("ren = %d 1表示成功　其他表示失败\n",ren);
        ren = write_PLC_serial(PLC_fd,buf,4,400,60);
        
        while(4 != ren)
        {
            sleep(1);
            ren = write_PLC_serial(PLC_fd,buf,4,400,60);
        }
        printf("ren = %d 1表示成功　其他表示失败\n",ren);
        sleep(20);
//        printf("ren = %d 1表示成功　其他表示失败\n",ren);
    }

    cloceSpectrum(watchdog_fd);
    return 0;
}
