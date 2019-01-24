/*************************************************************************
	> File Name: 2475-main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年01月03日 星期四 17时03分30秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<unistd.h>

#include"watchdog_2475.h"
#include"Data_Delivery_Cloud.h"
#include"ReuseFunction.h"


#define IP "http://39.105.67.7/LWI-getVersion.php"
#define DATA_IP "http://39.105.67.7/LWI-pushData.php"


int main(void)
{
    int fd;
    int ren;
    int i;
    spectrum data;
    char Sdata[200] = {0};
    char Ldata[20] = {0};
    i = 0;
    fd = initSpectrum("/dev/ttyO3",9600,8,0,1);   //初始化串口
    while(fd <= 0)
    {
        
        printf("文件名=%s,函数=%s,行号=%d 返回值 = %d error : initSpectrum fail!!!\n",__FILE__,__FUNCTION__,__LINE__,fd);
        sleep(1);
        i++;
        if(i >= 10)
        {
            printf("文件名=%s,函数=%s,行号=%d,返回值 = %d error : initSpectrum Implementitaon timeout!!!\n",__FILE__,__FUNCTION__,__LINE__,fd);  
        }
        fd = initSpectrum("/dev/ttyO2",9600,8,0,1);   
        
    }
    printf("文件名=%s,函数=%s,行号=%d,返回值 = %d correct : initSpectrum success!!!\n",__FILE__,__FUNCTION__,__LINE__,fd);  
    
    i = 0;
    while(init_Data_Delivery_Cloud(IP) < 0)
    {
        printf("文件名=%s,函数=%s,行号=%d error : init_Data_Delivery_Cloud fail!!!\n",__FILE__,__FUNCTION__,__LINE__);
        sleep(1);
        i++;
        if(i >= 10)
        {
            printf("文件名=%s,函数=%s,行号=%d error : init_Data_Delivery_Cloud Implementitaon timeout!!!\n",__FILE__,__FUNCTION__,__LINE__);
            return -1;
        }
    }
    printf("文件名=%s,函数=%s,行号=%d correct : init_Data_Delivery_Cloud success!!!\n",__FILE__,__FUNCTION__,__LINE__);
    while(1)
    {
        ren = read_Spectrum(fd,&data);
        if(ren < 0)
        {
            printf("文件名=%s,函数=%s,行号=%d,返回值=%d error : read_Spectrum read fail!!!\n",__FILE__,__FUNCTION__,__LINE__,ren);
            sleep(60);
            continue;
        }
        printf("文件名=%s,函数=%s,行号=%d,返回值=%d correct : read_Spectrum read success!!!\n",__FILE__,__FUNCTION__,__LINE__,ren);
        
        printf("时间 : %s\n",data.time);
        printf("电量 : %s\n",data.battery);
        printf("光照 : %s\n",data.PB);
        printf("湿度 : %s\n",data.PC);
        printf("温度 : %s\n",data.PD);
        
        memset(Ldata,0,20);
        TemperatureConversionFunction(data.PD,Ldata);
        printf("摄氏温度 : %s\n",Ldata);

        sprintf(Sdata,"RS;%s;20180016;---;247513446;40;%s;4;3;A;0;3;a;%s;A;1;37;a;%s;A;2;38;a;%s;1d4c",data.time,data.battery,Ldata,data.PC,data.PB);
        printf("数据 : %s\n",Sdata);
        comply_Data_Delivery_Cloud(DATA_IP,Sdata);

        printf("-------------------------\n");
        sleep(1800);
    }

    
    return 0;
}


